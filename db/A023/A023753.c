/*
 * A023753  Plaindromes: numbers whose digits in base 12 are in nondecreasing
 *          order.
 * https://oeis.org/A023753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 3081, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n-th integer whose digits in base 12 are in
 * nondecreasing order (plaindromes).  The sequence is indexed
 * starting at 1, with a(1)=0, a(2)=1, …
 */
#include <stdint.h>
#include <stdlib.h>

static idx_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n) {
    /* Single‑digit base‑12 numbers: 0..11 */
    if (n <= 12)
        return (seq_t)(n - 1);

    /* Find length L of the n‑th number (n > 12) */
    idx_t remain = n - 12;
    idx_t L = 2;
    for (;;) {
        /* count of L‑digit plaindromes with first digit ≥1:
           C(L+10,10) */
        idx_t cnt = binom_small(L + 10ULL, 10ULL);
        if (remain <= cnt)
            break;
        remain -= cnt;
        ++L;
    }

    /* Unrank the remain‑th sequence among length‑L plaindromes.
       digits[0] is the most significant digit. */
    uint8_t digits[32]; /* L ≤ 6 for n ≤ 10000, but allow more */
    idx_t idx = remain;
    idx_t prev = 1; /* minimal allowed digit for the first position */
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;   /* remaining positions after pos */
        for (int v = (int)prev; v <= 11; ++v) {
            idx_t ways;
            if (rem == 0)
                ways = 1;
            else
                ways = binom_small(rem + 11ULL - (idx_t)v, rem);
            if (idx <= ways) {
                digits[pos] = (uint8_t)v;
                prev = v;
                break;
            } else
                idx -= ways;
        }
    }

    /* Convert the base‑12 digits to a decimal value in seq_t. */
    seq_t result = 0;
    for (idx_t pos = 0; pos < L; ++pos)
        result = result * (seq_t)12 + (seq_t)digits[pos];

    return result;
}
