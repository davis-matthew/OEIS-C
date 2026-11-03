/*
 * A109040  Expansion of 1-eta(q)*eta(q^3)*(eta(q^4)*eta(q^6))^2/eta(q^12)^2
 *          in powers of q.
 * https://oeis.org/A109040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   multiplicative, a(1)=1; a(2^e)=a(3^e)=1; for p≠2,3 * if p≡1 or
 *            11 (mod 12): a(p^e) = (p^(e+1)-1)/(p-1) (positive geometric sum)
 *            * if p≡5 or 7 (mod 12): a(p^e) = ((-p)^(e+1)-1)/(-p-1)
 *            (alternating sum of powers of p) * a(n) = ∏ a(p_i^e_i) over the
 *            prime factorisation of n
 * Range:     i8 n <= 120, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9874 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: compute contribution of a prime power p^e */
static seq_t prime_contrib(idx_t p, int e)
{
    if (p == 2 || p == 3)
        return 1; /* for all exponents */

    /* determine sign pattern based on p mod 12 */
    int mod12 = (int)(p % 12);
    seq_t sum   = 1;          /* term p^0 = 1 */
    seq_t powp  = 1;          /* current power p^k */
    int sign    = 1;          /* sign for positive case */

    if (mod12 == 1 || mod12 == 11) {
        /* all terms positive */
        for (int k = 1; k <= e; ++k) {
            powp *= (seq_t)p;    /* p^k */
            sum  += powp;        /* sum += p^k */
        }
    } else { /* mod12 == 5 or 7 */
        /* alternating signs: +1, -p, +p^2, -p^3, ... */
        for (int k = 1; k <= e; ++k) {
            powp *= (seq_t)p;
            sign = -sign;            /* flip sign */
            sum  += (seq_t)sign * powp;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but return 0 for completeness */
        return 0;

    seq_t result = 1;
    idx_t temp   = n;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_contrib(p, e);
        }
    }

    /* remaining factor > 1 is prime */
    if (temp > 1)
        result *= prime_contrib(temp, 1);

    return result;
}
