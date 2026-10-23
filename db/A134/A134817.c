/*
 * A134817  a(1)=1; a(n)=a(n-1)*(number of decimal digits of a(n-1))+1.
 * https://oeis.org/A134817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(n)=a(n-1)*(number of decimal digits of a(n-1))+1
 *            (n>=2)
 * Range:     u8 n <= 14, u16 n <= 18, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the decimal digits of a value of type seq_t.
   Works for both built-in integer types and arbitrary-precision classes. */
static int digit_count(seq_t x)
{
    int cnt = 0;
    while (x > (seq_t)0) {
        x /= (seq_t)10;
        cnt++;
    }
    return cnt ? cnt : 1;   /* invariant: cnt==0 only if x==0, not expected for this sequence */
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n<1; return 0 as a sentinel */
        return (seq_t)0;
    }

    seq_t value = (seq_t)1;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        int d = digit_count(value);
        value = value * (seq_t)d + (seq_t)1;
    }
    return value;
}
