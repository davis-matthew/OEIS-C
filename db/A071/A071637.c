/*
 * A071637  Largest exponent k >=0 such that (n+1)^k divides n!.
 * https://oeis.org/A071637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest k such that (n+1)^k divides n!
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline seq_t legendre_exponent(idx_t n, idx_t p) {
    seq_t exp = 0;
    for (idx_t power = p; power <= n; ) {
        exp += n / power;
        if (power > n / p) break;          /* prevent overflow */
        power *= p;
    }
    return exp;
}

seq_t a(idx_t n) {
    idx_t m = n + 1;                        /* divisor to test              */
    seq_t result = (seq_t)-1;               /* placeholder, will be set      */
    bool first = true;

    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t exp_p = legendre_exponent(n, p);
            seq_t val = exp_p / (seq_t)e;   /* allowable exponent for this prime */
            if (first) {
                result = val;
                first = false;
            } else if (val < result) {
                result = val;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        seq_t exp_p = legendre_exponent(n, temp);
        seq_t val = exp_p;                  /* exponent in n! (p^1 factor)     */
        if (first) {
            result = val;
        } else if (val < result) {
            result = val;
        }
    }

    if (first) return (seq_t)0;  /* rare case n=0 → m=1, but offset starts at 1 */
    return result;
}
