/*
 * A054792  Earliest sequence with a(a(n)) = n^3.
 * https://oeis.org/A054792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(0)=0,a(1)=1; if n is a cube then a(n)=a(n^(1/3))^3; otherwise
 *            let m be the greatest cube < n. If n-m is odd then a(n)=n+1,
 *            else a(n)=(n-1)^3
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_cube_root.h"   /* provides idx_t int_cube_root(idx_t x) */

seq_t a(idx_t n)
{
    /* Base cases for small indices */
    if (n == 0U) return (seq_t)0;
    if (n == 1U) return (seq_t)1;

    /* Check whether n is a perfect cube. */
    idx_t k = int_cube_root(n);              /* floor cube root of n */
    if ((k * k * k) == n) {
        /* Recursive case: n = k^3 */
        seq_t val = a(k);                    /* a(k) */
        return val * val * val;              /* a(k)^3 */
    }

    /* n is not a cube. Find the greatest cube < n. */
    idx_t r = int_cube_root(n - 1);          /* floor cube root of n-1 */
    idx_t cube_less = r * r * r;             /* r^3 < n */
    idx_t diff = n - cube_less;              /* n - r^3 */

    if (diff & 1U) {                         /* difference is odd */
        return (seq_t)n + (seq_t)1;          /* a(n) = n + 1 */
    } else {                                 /* difference is even */
        /* a(n) = (n-1)^3 */
        idx_t m = n - 1U;
        seq_t mm = (seq_t)m;
        return mm * mm * mm;                 /* (n-1)^3 */
    }
}
