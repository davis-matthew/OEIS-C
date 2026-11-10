/*
 * A051552  Quotients of consecutive values of LCM {b(0), b(1) ...,b(n)}, b()
 *          = A016789.
 * https://oeis.org/A051552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = A051540(n)/A051540(n-1) = (3n+2)/gcd(lcm{3k+2 : 0<=k<n},
 *            3n+2); computed by factoring 3n+2 and checking, for each prime
 *            power, whether an earlier term 3k+2 (k<n) is divisible by it
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9926 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* smallest term of form 3k+2 that is a multiple of x (x not divisible by 3) */
static seq_t A051552_smallest_mult(seq_t x)
{
    if (x % 3 == 2) return x;
    return x * 2;
}

static seq_t A051552_part(seq_t p, seq_t pe, seq_t t)
{
    seq_t res = 1;
    seq_t pf = pe;
    while (pf > 1 && A051552_smallest_mult(pf) >= t) {
        pf /= p;
        res *= p;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 3 + 2;
    seq_t rem = t;
    seq_t result = 1;
    seq_t p;
    for (p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            seq_t q = 1;
            while (rem % p == 0) {
                rem /= p;
                q *= p;
            }
            result *= A051552_part(p, q, t);
        }
    }
    if (rem > 1) {
        result *= A051552_part(rem, rem, t);
    }
    return result;
}
