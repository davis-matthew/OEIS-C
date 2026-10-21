/*
 * A052273  Number of distinct 4th powers mod n.
 * https://oeis.org/A052273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(p^e) = 1 + floor((p-1)*p^(e+3)/(gcd(p-1,4)*(p^4-1))) for odd
 *            primes p and a(2^e) = 1 + floor(2^e/15) when e == 0 (mod 4) else
 *            2 + floor(2^e/15)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, gcd, int_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative: a(n) = Π a(p^e) over the prime-power
   factorisation of n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

static seq_t prime_power_val(idx_t p, idx_t e)
{
    if (p == 2) {
        /* a(2^e) */
        seq_t pow2e   = pow_uint(2ULL, e);
        seq_t base    = pow2e / 15;            /* floor(2^e / 15) */
        if ((e % 4ULL) == 0ULL)
            return 1 + base;
        else
            return 2 + base;
    } else {
        /* odd prime */
        seq_t g        = gcd((seq_t)(p - 1), (seq_t)4);
        seq_t a_factor = (seq_t)(p - 1) / g;   /* (p-1)/gcd(p-1,4) */

        /* compute p^(e+3) and p^4 - 1 carefully */
        seq_t pow_p_e4 = pow_uint(p, e + 3);        /* p^(e+3) */
        seq_t denom    = pow_uint(p, 4) - 1;        /* p^4 - 1 */
        seq_t div_up   = pow_p_e4 / denom;          /* floor division */

        seq_t term = a_factor * div_up;
        return term + 1;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;   /* a(1) = 1 */

    seq_t result   = 1;
    idx_t temp_n   = n;

    idx_t sqrt_n   = int_sqrt(n);
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(sqrt_n, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt && temp_n > 1; ++i) {
        uint64_t p_val = primes[i];
        idx_t     p     = (idx_t)p_val;
        if (p * p > temp_n)
            break;
        if (temp_n % p == 0) {
            idx_t e = 0;
            while (temp_n % p == 0) {
                temp_n /= p;
                ++e;
            }
            result *= prime_power_val(p, e);
        }
    }

    /* Remaining prime factor > sqrt(original) */
    if (temp_n > 1) {
        result *= prime_power_val(temp_n, 1);
    }

    free(primes);
    return result;
}
