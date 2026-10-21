/*
 * A060488  Number of 4-block ordered tricoverings of an unlabeled n-set.
 * https://oeis.org/A060488
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*(n+9)/6
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* values below the offset are undefined */

    /* Use a 64‑bit intermediate to avoid overflow for 32‑bit seq_t */
    uint64_t t1 = (uint64_t)(n - 2);
    uint64_t t2 = (uint64_t)(n - 1);
    uint64_t t3 = (uint64_t)(n + 9);

    uint64_t num = t1 * t2 * t3;   /* t1*t2*t3 fits into 64 bits for n <= 10000 */
    num /= 6;                      /* division is exact */

    return (seq_t)num;
}
