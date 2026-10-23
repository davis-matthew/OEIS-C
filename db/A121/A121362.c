/*
 * A121362  Expansion of
 *          eta(q)*eta(q^6)*eta(q^10)*eta(q^15)/(eta(q^3)*eta(q^5)) in powers
 *          of q.
 * https://oeis.org/A121362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e of * a(p^e) = { (-1)^e if p
 *            is 2,3,5 * e+1 if p ≡ 1 or 4 (mod 15) * (-1)^e (e+1) if p ≡ 2 or
 *            8 (mod 15) * (1 + (-1)^e)/2 if p ≡ 7,11,13,14 (mod 15) }
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* compute value for a single prime power p^exp */
static seq_t prime_power_value(idx_t p, idx_t exp)
{
    if (p == 2 || p == 3 || p == 5) {
        return (exp % 2 == 0) ? 1 : -1;
    }
    int mod = (int)(p % 15);
    if (mod == 1 || mod == 4) {
        return (seq_t)(exp + 1);
    } else if (mod == 2 || mod == 8) {
        return ((exp % 2 == 0) ? 1 : -1) * (seq_t)(exp + 1);
    } else { /* 7,11,13,14 */
        return (exp % 2 == 0) ? 1 : 0;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t result = 1;
    idx_t exp = 0;

    /* factor out 2 */
    while ((n & 1ULL) == 0) {
        n >>= 1;
        exp++;
    }
    if (exp) {
        result *= prime_power_value(2, exp);
        if (result == 0)
            return 0;
    }

    /* factor out 3 */
    exp = 0;
    while (n % 3ULL == 0) {
        n /= 3ULL;
        exp++;
    }
    if (exp) {
        result *= prime_power_value(3, exp);
        if (result == 0)
            return 0;
    }

    /* factor out 5 */
    exp = 0;
    while (n % 5ULL == 0) {
        n /= 5ULL;
        exp++;
    }
    if (exp) {
        result *= prime_power_value(5, exp);
        if (result == 0)
            return 0;
    }

    /* factor remaining odd primes using 6k±1 */
    for (idx_t p = 7; p <= n / p; p += 6) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            result *= prime_power_value(p, exp);
            if (result == 0)
                return 0;
        }
        idx_t p2 = p + 2;
        if (p2 > n / p2)
            break;
        if (n % p2 == 0) {
            exp = 0;
            while (n % p2 == 0) {
                n /= p2;
                exp++;
            }
            result *= prime_power_value(p2, exp);
            if (result == 0)
                return 0;
        }
    }

    /* if remaining n is a prime > 1 */
    if (n > 1) {
        result *= prime_power_value(n, 1);
    }

    return result;
}
