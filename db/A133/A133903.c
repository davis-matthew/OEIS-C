/*
 * A133903  a(n) = a(n-1) + A000005(n-1) if a(n-1) is not divisible by 3,
 *          otherwise a(n) = a(n-1)/3.
 * https://oeis.org/A133903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1 ; for n>1: a(n)=a(n-1)+d(n-1) if a(n-1)%3!=0
 *            a(n)=a(n-1)/3 if a(n-1)%3==0 where d(k)=A000005(k)=number of
 *            divisors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t num_divisors(idx_t n)
{
    if (n <= 1)
        return 1;          /* d(1)=1; n=0 does not occur here */
    seq_t cnt = 1;
    idx_t temp = n;
    idx_t p = 2;
    while (p <= temp / p) {
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            cnt *= (exp + 1);
        }
        ++p;
    }
    if (temp > 1)
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of range, not used in tests */
    seq_t val = 1;         /* a(1) */
    for (idx_t k = 1; k < n; ++k) {
        seq_t d = num_divisors(k);
        if (val % 3 == 0)
            val /= 3;
        else
            val += d;
    }
    return val;
}
