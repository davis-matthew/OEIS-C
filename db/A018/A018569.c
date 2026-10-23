/*
 * A018569  Divisors of 646.
 * https://oeis.org/A018569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Divisors of 646, sorted in ascending order */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 or n > 8 (the number of divisors of 646) */
    if (n < 1) return 0;

    /* Factorize 646 = 2 * 17 * 19 */
    uint64_t m = 646;
    uint64_t fac[3];
    int fcnt = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            fac[fcnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) fac[fcnt++] = m;          /* remaining prime factor */

    /* There are 2^fcnt = 8 divisors for this number */
    const int total = 1 << fcnt;         /* equals 8 */
    seq_t divs[8];

    /* Generate all divisors by bit masks */
    for (int mask = 0; mask < total; ++mask) {
        seq_t prod = 1;
        for (int i = 0; i < fcnt; ++i) {
            if (mask & (1 << i))
                prod *= (seq_t)fac[i];
        }
        divs[mask] = prod;
    }

    /* Sort the divisors in ascending order (simple insertion sort) */
    for (int i = 1; i < total; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    if (n > (idx_t)total) return 0;          /* out of bounds */
    return divs[(int)(n - 1)];
}
