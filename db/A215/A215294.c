/*
 * A215294  Number of permutations of 0..floor((n*3-2)/2) on odd squares of an
 *          n X 3 array such that each row and column of odd squares is
 *          increasing.
 * https://oeis.org/A215294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   _pow_prime, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of SYT of shape (n,n,n)
 *   for n even (n=2k): a(n) = 6*(3k-1)! / ((2+2k)*k!^2*(k-1)!)
 *   for n odd  (n=2k+1): a(n) = (k+1)*(3k+1)! / ((k+1)!^3)
 *   implemented by prime‑factorisation to avoid overflow.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/_pow_prime.h"

static void _init_primes(void);
static idx_t _legendre(idx_t n, idx_t p);
static seq_t _pow_prime(idx_t p, idx_t exp);

/* Prime table initialisation */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static bool prime_inited = false;

static inline void _init_primes(void)
{
    if (prime_inited) return;
    prime_inited = true;
    /* enough to cover 3*ceil((max_n+1)/2)-2 <= 1.5*max_n+1;
       for max_n ≈ 10000, use 20000. */
    build_primes_upto(20000, &prime_tbl, &prime_cnt);
}

/* Legendre exponent of prime p in n! */
static idx_t _legendre(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    _init_primes();
    seq_t result = 1;

    if (n & 1) {                     /* odd n = 2k+1 */
        idx_t m = (n + 1) >> 1;      /* m = (n+1)/2 */
        idx_t num_fact = 3 * m - 2;   /* 3m-2 */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_tbl[i];
            if (p > num_fact) break;
            /* exponent of p in (3m-2)! */
            idx_t e_num = _legendre(num_fact, p);
            /* exponent of p in m!  */
            idx_t e_den = 3 * _legendre(m, p);
            /* add exponent of p in m itself */
            idx_t tmp = m;
            idx_t e_m = 0;
            while (tmp % p == 0) {
                ++e_m;
                tmp /= p;
            }
            idx_t exp = e_num - e_den + e_m;
            if (exp > 0) {
                result *= _pow_prime(p, exp);
            }
        }
    } else {                          /* even n = 2k */
        idx_t k = n >> 1;
        idx_t num_fact = 3 * k - 1;   /* 3k-1 */
        idx_t n2 = n + 2;             /* 2+2k = n+2 */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_tbl[i];
            if (p > num_fact && p > n2) break;
            /* exponent of p in numerator (3k-1)! */
            idx_t e_num = _legendre(num_fact, p);
            /* exponents in denominator */
            idx_t e_k = _legendre(k, p);
            idx_t e_k1 = _legendre(k - 1, p);
            idx_t e_den = 2 * e_k + e_k1;
            /* add exponents of 6 = 2*3 */
            idx_t add6 = 0;
            if (p == 2) add6 += 1;
            if (p == 3) add6 += 1;
            /* subtract exponents of (n+2) */
            idx_t n2_temp = n2;
            idx_t e_n2 = 0;
            while (n2_temp % p == 0) {
                ++e_n2;
                n2_temp /= p;
            }
            idx_t exp = e_num - e_den + add6 - e_n2;
            if (exp > 0) {
                result *= _pow_prime(p, exp);
            }
        }
    }
    return result;
}
