/*
 * A133058  a(0) = a(1) = 1; for n > 1, a(n) = a(n-1) + n + 1 if a(n-1) and n
 *          are coprime, otherwise a(n) = a(n-1)/gcd(a(n-1),n).
 * https://oeis.org/A133058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, nice, easy
 * Formula:   a(0)=1, a(1)=1; for n>1, a(n)=a(n-1)+n+1 if gcd(a(n-1),n)=1,
 *            else a(n)=a(n-1)/gcd(a(n-1),n);
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* harmless, supports seq_t types */

/* local GCD – works for any seq_t that supports % and == */
static seq_t gcd_local(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* start from a(1) and iterate */
    seq_t prev = (seq_t)1;              /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd_local(prev, (seq_t)i);
        if (g == (seq_t)1)
            prev = prev + (seq_t)i + (seq_t)1;
        else
            prev = prev / g;
    }
    return prev;
}
