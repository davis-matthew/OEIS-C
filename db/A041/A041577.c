/*
 * A041577  Denominators of continued fraction convergents to sqrt(306).
 * https://oeis.org/A041577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 70*a(n-2) - a(n-4) for n>3, a(0) = 1, a(1) = 2, a(2) =
 *            69, a(3) = 140
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)69;
    if (n == 3) return (seq_t)140;

    /* rolling array: arr[0]=a(k-1), arr[1]=a(k-2),
       arr[2]=a(k-3), arr[3]=a(k-4) */
    seq_t arr[4];
    arr[0] = (seq_t)140; /* a(3) */
    arr[1] = (seq_t)69;  /* a(2) */
    arr[2] = (seq_t)2;   /* a(1) */
    arr[3] = (seq_t)1;   /* a(0) */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t val = (seq_t)70 * arr[1] - arr[3];
        /* shift: new a(k) becomes arr[0], a(k-1) becomes arr[1], etc. */
        arr[3] = arr[2];
        arr[2] = arr[1];
        arr[1] = arr[0];
        arr[0] = val;
    }
    return arr[0];
}
