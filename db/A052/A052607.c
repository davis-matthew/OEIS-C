/*
 * A052607  E.g.f. (1-x^3)/(1-x^2-x^3).
 * https://oeis.org/A052607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * k(n) * where k(0)=1, k(1)=0, k(2)=1, k(3)=0 and for
 *            n ≥ 4 * k(n) = k(n-2) + k(n-3). * The sequence values a(n) are
 *            given by a(0)=1, a(1)=0, a(2)=2, a(3)=0, * a(4)=24, a(5)=120,
 *            a(6)=720, a(7)=10080, a(8)=80640, …
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;   /* 2! * k(2)=2*1 */
    if (n == 3) return (seq_t)0;

    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute k(n) with a small ring buffer of size 4 */
    seq_t k[4];
    k[0] = (seq_t)1;   /* k(0) */
    k[1] = (seq_t)0;   /* k(1) */
    k[2] = (seq_t)1;   /* k(2) */
    k[3] = (seq_t)0;   /* k(3) */

    for (idx_t i = 4; i <= n; ++i)
        k[(size_t)(i % 4)] = k[(size_t)((i - 2) % 4)] + k[(size_t)((i - 3) % 4)];

    return fact * k[(size_t)(n % 4)];
}
