/*
 * A127971  a(n) = fusc(n+1) + (1-(-1)^n)/2, fusc = A002487.
 * https://oeis.org/A127971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fusc(n+1) + (1-(-1)^n)/2 * where fusc is Stern's diatomic
 *            series: * fusc(0)=0, fusc(1)=1; * fusc(2k)=fusc(k); *
 *            fusc(2k+1)=fusc(k)+fusc(k+1). * * a(n) = fusc(n+1) + n%2
 * Range:     u8 n <= 4689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9565 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t fusc_n_plus_one(idx_t n)
{
    seq_t x = 0, y = 1;                /* (x, y) = (fusc(0), fusc(1)) */
    idx_t msb = 0, t = n;              /* number of bits in n */
    while (t) { msb++; t >>= 1; }

    /* iterate over bits of n from the most significant to the least */
    for (idx_t i = msb; i-- > 0; ) {
        if ((n >> i) & 1) {
            seq_t tmp = x + y;
            x = tmp;                  /* odd case: fusc(2k+1)=x+y */
        } else {
            y = x + y;                 /* even case: fusc(2k)=y */
        }
    }
    /* after the loop y = fusc(n+1) */
    return y;
}

seq_t a(idx_t n)
{
    /* fusc(n+1) is obtained by passing n into the iterative routine */
    seq_t f = fusc_n_plus_one(n);
    seq_t parity = n & 1;                /* (1-(-1)^n)/2 is 1 for odd n, 0 for even */
    return f + parity;
}
