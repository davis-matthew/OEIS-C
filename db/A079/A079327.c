/*
 * A079327  Smallest nonnegative integer x such that b^(n-1) == b^x (mod n)
 *          for all b that are 0 < b < n.
 * https://oeis.org/A079327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest nonnegative integer x such that b^(n-1) == b^x
 *            (mod n) for all 0 < b < n
 * Range:     u8 n <= 368, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   gcd, is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

/* Return lcm of two seq_t values (a / gcd(a,b) * b). */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Primes have the value 0. */
    if (is_prime(n))
        return (seq_t)0;

    idx_t temp = n;
    idx_t max_exp = 0;
    seq_t lambda_total = (seq_t)1;   /* λ of the current factorization. */

    /* Factor out the power of 2 first. */
    if (temp % (idx_t)2 == 0) {
        idx_t exp = 0;
        while (temp % (idx_t)2 == 0) {
            temp /= (idx_t)2;
            ++exp;
        }
        if (exp > 0) {
            seq_t lam;
            if (exp == 1)
                lam = (seq_t)1;                       /* λ(2) = 1 */
            else if (exp == 2)
                lam = (seq_t)2;                       /* λ(4) = 2 */
            else /* exp >= 3 */
                lam = pow_uint((uint64_t)2, exp - (idx_t)2);  /* λ(2^e) = 2^{e-2} */

            lambda_total = lcm_seq(lambda_total, lam);
            if (exp > max_exp)
                max_exp = exp;
        }
    }

    /* Trial division for odd primes. */
    for (idx_t p = (idx_t)3; p <= temp / p; p += (idx_t)2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_pow_e       = pow_uint(p, exp);
            seq_t p_pow_e_minus = pow_uint(p, exp - (idx_t)1);
            seq_t lam = p_pow_e - p_pow_e_minus;   /* λ(p^e) = p^{e-1}(p-1) */

            lambda_total = lcm_seq(lambda_total, lam);
            if (exp > max_exp)
                max_exp = exp;
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > (idx_t)1) {
        /* Here exp = 1. */
        seq_t lam = (seq_t)(temp - (idx_t)1);   /* λ(p) = p-1 */
        lambda_total = lcm_seq(lambda_total, lam);
        if ((idx_t)1 > max_exp)
            max_exp = (idx_t)1;
    }

    /* Compute (n-1) mod λ. */
    seq_t r = (seq_t)(n - (idx_t)1) % lambda_total;

    /* Ensure x ≥ max_exp (to handle multipliers of prime factors). */
    seq_t x;
    if (r >= (seq_t)max_exp) {
        x = r;
    } else {
        seq_t diff = (seq_t)max_exp - r;
        seq_t t = (diff + lambda_total - (seq_t)1) / lambda_total;   /* ceil division */
        x = r + t * lambda_total;
    }

    return x;
}
