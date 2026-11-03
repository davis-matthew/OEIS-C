/*
 * A143323  Expansion of eta(q^2)^4 * eta(q^5) * eta(q^20)^2 / ( eta(q) *
 *          eta(q^4)^2 * eta(q^10)^2 ) in powers of q.
 * https://oeis.org/A143323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   multiplicative with prime power rules a(2^e) = -(-1)^e (e≥1,
 *            a(1)=1) a(5^e) = 1 a(p^e) = 1 if p≡5 (mod20) a(p^e) =
 *            (1+(-1)^e)/2 if p≡11,13,17,19 (mod20) a(p^e) = e+1 if p≡1,9
 *            (mod20) a(p^e) = (e+1)(-1)^e if p≡3,7 (mod20) The function
 *            computes a(n) for n≥1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 947 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t prime_value(idx_t p, idx_t e)
{
    /* Handle p == 2 separately (e>=1). For e==0, product part will
       never be called; a(1) is handled in the main function. */
    if (p == 2) {
        /* e is at least 1 here. */
        return (e & 1) ? (seq_t)1 : (seq_t)(-1);
    }
    if (p == 5) {
        return 1;
    }
    idx_t mod = p % 20;
    if (mod == 11 || mod == 13 || mod == 17 || mod == 19) {
        /* (1+(-1)^e)/2 : 1 if e even, 0 if e odd */
        return (e & 1) ? (seq_t)0 : (seq_t)1;
    } else if (mod == 1 || mod == 9) {
        return (seq_t)(e + 1);
    } else if (mod == 3 || mod == 7) {
        seq_t val = (seq_t)(e + 1);
        if (e & 1) val = -val;
        return val;
    } else {
        /* Should not happen for primes >5, but fall back safely. */
        return 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* off‑by‑one safety, not used */
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_value(p, e);
            if (result == 0) return 0;   /* early exit */
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        result *= prime_value(temp, 1);
    }
    return result;
}
