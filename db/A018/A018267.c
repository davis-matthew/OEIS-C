/*
 * A018267  Divisors of 63.
 * https://oeis.org/A018267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 63; a(n) is the n-th divisor in ascending order. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* below offset, undefined */

    const seq_t m = (seq_t)63;    /* the number whose divisors we list */
    idx_t i;
    seq_t temp[12];
    unsigned int cnt = 0;

    for (i = 1; (seq_t)i * (seq_t)i <= m; ++i) {
        seq_t t = (seq_t)i;
        if (m % t == 0) {
            temp[cnt++] = t;
            seq_t other = m / t;
            if (other != t)
                temp[cnt++] = other;
        }
    }

    /* simple insertion sort of the divisor list */
    unsigned int j, k;
    for (j = 1; j < cnt; ++j) {
        seq_t key = temp[j];
        k = j;
        while (k > 0 && temp[k - 1] > key) {
            temp[k] = temp[k - 1];
            --k;
        }
        temp[k] = key;
    }

    if (n > cnt)
        return (seq_t)0;      /* no such divisor beyond the list */
    return temp[(size_t)(n - 1)];
}
