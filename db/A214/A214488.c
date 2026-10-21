/*
 * A214488  List of words over {1,3} with equal numbers of 1's and 3's.
 * https://oeis.org/A214488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 350, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214488: words over {1,3} with equal numbers of 1's and 3's
   sorted lexicographically (base 10).
   The construction uses combinatorial unranking of binary strings
   with k zeros (mapped to 1) and k ones (mapped to 3). */

#include <stdint.h>  /* for uint64_t used by types idx_t, seq_t */

static seq_t binom_seq(idx_t n, idx_t k) {
    /* Compute C(n,k) exactly using multiplicative formula. */
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, but harmless */

    seq_t target = (seq_t)n;
    seq_t cum = 0;
    idx_t k = 0;

    /* Find the length 2k that contains the nth word. */
    while (cum < target) {
        ++k;
        cum += binom_seq(2 * k, k);
    }

    seq_t prev = cum - binom_seq(2 * k, k); /* count before this group */
    seq_t offset = target - prev - 1;        /* 0‑based rank within group */

    /* Unrank the offset to a binary string of length 2k with k zeros. */
    seq_t result = 0;
    idx_t zeros_left = k;

    for (idx_t pos = 0; pos < 2 * k; ++pos) {
        idx_t remaining_positions = 2 * k - pos - 1;
        seq_t countZero = 0;

        if (zeros_left > 0) {
            countZero = binom_seq(remaining_positions, zeros_left - 1);
        }

        if (zeros_left > 0 && offset < countZero) {
            /* place a zero -> '1' digit */
            result = result * 10 + 1;
            --zeros_left;
        } else {
            if (zeros_left > 0) offset -= countZero;
            /* place a one -> '3' digit */
            result = result * 10 + 3;
        }
    }

    return result;
}
