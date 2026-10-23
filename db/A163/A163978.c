/*
 * A163978  a(n) = 2*a(n-2) for n > 2; a(1) = 3, a(2) = 4.
 * https://oeis.org/A163978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n > 2; a(1) = 3, a(2) = 4
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Return the n-th term of sequence A163978. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    /* For odd n = 2*k-1, a(n) = 3 * 2^(k-1)
       For even n = 2*k,   a(n) = 4 * 2^(k-1)                        */
    idx_t k = (n + (n % 2 ? 0 : 0)) / 2;  /* compute floor(n/2) for both cases */
    if (n % 2) {            /* odd */
        k = (n + 1) / 2;    /* k = (n+1)/2 */
        return (seq_t)3 * pow2(k - 1);
    } else {                /* even */
        k = n / 2;          /* k = n/2 */
        return (seq_t)4 * pow2(k - 1);
    }
}
