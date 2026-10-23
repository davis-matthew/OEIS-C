/*
 * A117212  Sum_{d|n} a(d)/d = (-1)^(n-1)/n for n>=1; equals the logarithmic
 *          g.f. of A117210.
 * https://oeis.org/A117212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A117212:
 *   a(n) = Σ_{d|n} (-1)^(n/d-1) * μ(d) * d
 *   μ(d) is the Möbius function.
 *   The implementation enumerates all divisors of n from its prime
 *   factorisation.  It works for all n >= 1 and any integer type
 *   seq_t/idx_t supplied by the build.  No external utilities are
 *   required; a simple trial division factorises n up to 10 000.
 */

#include <stdint.h>
#include <stdbool.h>

static void rec_divisors(idx_t idx, idx_t total,
                         idx_t prime[], idx_t exp[],
                         idx_t cur, int mu_sign,
                         idx_t n, seq_t *sum)
{
    if (idx == total) {
        /* cur is a divisor d of n, with μ(cur)=mu_sign (0 if square‑free part wrong) */
        if (mu_sign == 0) return;            /* μ(cur)=0 -> term vanishes */
        seq_t d      = (seq_t)cur;
        seq_t nd     = (seq_t)n / d;          /* n/d */
        int sign1    = (nd & 1) ? 1 : -1;      /* (-1)^(n/d-1) */
        *sum += (seq_t)sign1 * (seq_t)mu_sign * d;
        return;
    }

    /* trial over exponent of prime[idx] from 0..exp[idx] */
    for (idx_t e = 0; e <= exp[idx]; ++e) {
        if (e == 0) {
            rec_divisors(idx + 1, total, prime, exp,
                         cur, mu_sign, n, sum);
        } else if (e == 1) {
            /* multiply by this prime once, flip mu_sign */
            rec_divisors(idx + 1, total, prime, exp,
                         cur * prime[idx], -mu_sign, n, sum);
        } else {
            /* e > 1 => μ=0, term vanishes, no need to proceed further */
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of OEIS range but harmless */

    /* factorise n by trial division */
    idx_t prime[16];
    idx_t exp[16];
    idx_t pcnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prime[pcnt] = p;
            exp[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        prime[pcnt] = temp;
        exp[pcnt]   = 1;
        ++pcnt;
    }

    seq_t sum = 0;
    rec_divisors(0, pcnt, prime, exp, 1, 1, n, &sum);
    return sum;
}
