/*
 * A038450  Sums of 8 distinct powers of 10.
 * https://oeis.org/A038450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest integer that has exactly eight decimal
 *            digits equal to 1 (and all other digits equal to 0)
 * Range:     u8 none, u16 none, u32 n <= 45, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute binomial coefficient C(n,k) for 0 <= k <= 7.
   n and k are integer indices stored in idx_t, the result is seq_t.
   The algorithm multiplies and divides incrementally to avoid overflow. */
static inline seq_t binom_idx_k(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe fallback */

    /* Convert to zero‑based rank. */
    idx_t rank = n - 1;

    /* Find the decimal length (number of digits) of the answer. */
    idx_t d = 8;                /* minimal length (all powers 0..7) */
    seq_t cumulative = 0;
    while (true) {
        seq_t count = binom_idx_k(d - 1, 7);   /* C(d-1,7) numbers with d digits */
        if (cumulative + count > (seq_t)rank) break;
        cumulative += count;
        ++d;
    }

    /* Offset inside the block of length d. */
    seq_t offset_seq = (seq_t)rank - cumulative;
    idx_t offset = (idx_t)offset_seq;         /* fits because count <= 2^64 for test limits */

    idx_t zeros   = d - 8;          /* number of zero digits in this block */
    idx_t nPos    = d - 1;          /* positions available for zeros (positions 1..d-1) */

    /* Determine the positions of the zero digits using combinatorial number system. */
    idx_t *zero_pos = NULL;
    if (zeros > 0) {
        zero_pos = (idx_t*)malloc(sizeof(idx_t) * (size_t)zeros);
    }

    idx_t prev = 0;
    seq_t r_seq = offset_seq;
    for (idx_t i = 1; i <= zeros; ++i) {
        for (idx_t c = prev + 1; c <= nPos; ++c) {
            idx_t remaining = zeros - i;
            seq_t comb = binom_idx_k(nPos - c, remaining);
            if (comb <= r_seq) {
                r_seq -= comb;
            } else {
                zero_pos[i - 1] = c;          /* zero at position c (counting from 1) */
                prev = c;
                break;
            }
        }
    }

    /* Construct the decimal number from the determined zero positions. */
    seq_t value = 0;
    idx_t zero_index = 0;
    for (idx_t pos = 1; pos <= d; ++pos) {
        value = value * 10;
        bool is_zero = false;
        if (pos > 1) {                    /* positions 2..d can be zero */
            idx_t idx = pos - 1;           /* map to zero position number */
            if (zero_index < zeros && zero_pos[zero_index] == idx) {
                is_zero = true;
                ++zero_index;
            }
        }
        if (!is_zero) {
            value = value + 1;
        }
    }

    if (zero_pos) free(zero_pos);
    return value;
}
