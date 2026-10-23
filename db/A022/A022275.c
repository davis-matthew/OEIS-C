/*
 * A022275  a(n) = n*(17*n + 1)/2.
 * https://oeis.org/A022275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(17*n + 1)/2
 * Range:     u8 n <= 5, u16 n <= 87, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A022275.
   a(0) = 0, a(1) = 9, a(n) = n*(17*n + 1)/2 for n > 0.
   This implementation avoids intermediate overflow by dividing
   by 2 before the final multiplication. */
seq_t a(idx_t n)
{
    /* Cast the index to the sequence type once */
    seq_t sn = (seq_t)n;
    seq_t result;

    /* The product n*(17*n+1) is always divisible by 2.
       To keep intermediate values small we divide by 2
       before the final multiplication:  (n/2)*(17*n+1) if n even,
       or   n * ((17*n+1)/2)           if n is odd. */
    if (sn & 1) {                /* sn is odd */
        seq_t t = ((seq_t)17 * sn + 1) >> 1; /* (17*sn + 1)/2 */
        result = sn * t;
    } else {                     /* sn is even */
        seq_t t = 17 * sn + 1;
        result = (sn >> 1) * t; /* (sn/2) * (17*sn + 1) */
    }

    return result;
}
