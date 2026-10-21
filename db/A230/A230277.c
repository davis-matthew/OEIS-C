/*
 * A230277  Expansion of f(-q^6)^6 * ( q * chi(-q^3)^4 + 2*q^2 / chi(-q^3)^4 )
 *          in powers of q where chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A230277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a multiplicative function with prime power rules a(1) = 1 a(2^e)
 *            = -(-2)^e a(3^e) = 0 For primes p ≡ 3 or 7 (mod 8): a(p^e) = p^e
 *            if e even, else 0 For primes p ≡ 1 or 5 (mod 8): Let p = x^2 +
 *            4y^2 (x,y integer, existing for such p). t = 2 * ( 4xy ) if p ≡
 *            5 (mod 6) = 2 * ( p - 8y^2 ) otherwise a...
 * Range:     i8 n <= 72, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   build_primes_upto, floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"
#include "../utils/floor_sqrt.h"

/* Compute a(p) for a prime p ≡ 1 or 5 (mod 8) */
static seq_t a_prime(uint64_t p)
{
    uint64_t x = 0, y = 0;
    /* search for representation p = x^2 + 4*y^2 */
    for (uint64_t yy = 0; 4 * yy * yy <= p; ++yy) {
        uint64_t rem = p - 4 * yy * yy;
        seq_t sq = floor_sqrt((seq_t)rem);
        if ((seq_t)sq * (seq_t)sq == (seq_t)rem) {
            x = (uint64_t)sq;
            y = yy;
            break;
        }
    }
    /* compute t */
    seq_t t;
    if (p % 6 == 5) {
        t = (seq_t)8 * (seq_t)x * (seq_t)y;       /* 2*4xy = 8xy */
    } else {
        t = (seq_t)2 * ((seq_t)p - (seq_t)8 * (seq_t)y * (seq_t)y);
    }
    /* sign = (-1)^(t mod 3)  */
    int mod3 = (int)(t % 3);
    if (mod3 < 0) mod3 += 3;
    seq_t sign = (mod3 == 1) ? (seq_t)(-1) : (seq_t)1;
    return t * sign;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* static prime table */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    if (!initialized) {
        /* enough to factor numbers up to at least 10^12 */
        build_primes_upto((idx_t)1000000, &primes, &prime_cnt);
        initialized = true;
    }

    idx_t temp = n;
    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t prime_val;
            if (p == 2) {
                /* a(2^e) = -(-2)^e */
                seq_t powneg2 = 1;
                for (int k = 0; k < e; ++k) powneg2 *= (seq_t)-2;
                prime_val = -powneg2;
            } else if (p == 3) {
                return 0;
            } else if (p % 8 == 3 || p % 8 == 7) {
                if (e % 2 == 0) {
                    prime_val = pow_seq((seq_t)p, (idx_t)e);
                } else {
                    return 0;
                }
            } else { /* p % 8 == 1 or 5 */
                seq_t aP = a_prime(p);
                if (e == 1) {
                    prime_val = aP;
                } else {
                    seq_t prev2 = 1;         /* a(p^0) */
                    seq_t prev1 = aP;        /* a(p^1) */
                    seq_t cur = 0;
                    seq_t pp = (seq_t)p * (seq_t)p;
                    for (int exp = 2; exp <= e; ++exp) {
                        cur = aP * prev1 - pp * prev2;
                        prev2 = prev1;
                        prev1 = cur;
                    }
                    prime_val = cur;
                }
            }
            result *= prime_val;
            if (result == 0) return 0;
        }
    }

    if (temp > 1) {            /* remaining prime factor > sqrt(original n) */
        uint64_t p = temp;
        seq_t prime_val;
        if (p == 2) {
            seq_t powneg2 = -2;    /* e = 1 */
            prime_val = -powneg2;  /* = 2 */
        } else if (p == 3) {
            return 0;
        } else if (p % 8 == 3 || p % 8 == 7) {
            return 0;              /* odd exponent gives 0 */
        } else { /* p % 8 == 1 or 5 */
            prime_val = a_prime(p);   /* e = 1 */
        }
        result *= prime_val;
    }

    return result;
}
