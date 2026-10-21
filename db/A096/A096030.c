/*
 * A096030  Values (y-x-1)/2, where x^2+y^2=p,(x<y) runs over the Pythagorean
 *          primes A002144.
 * https://oeis.org/A096030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (y - x - 1)/2, where p_n is the nth prime ≡ 1 (mod 4) and
 *            p_n = x² + y², x < y
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Primitive integer square root using 128‑bit multiplication. */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    for (int bit = 63; bit >= 0; --bit) {
        uint64_t t = r | (1ULL << bit);
        if ((unsigned __int128)t * t <= n)
            r = t;
    }
    return r;
}

/* Small prime sieve/composite test with incremental list of primes. */
static uint32_t *primelist = NULL;
static size_t primelist_cnt = 0;
static size_t primelist_cap = 0;
static uint32_t prime_candidate = 2;

/* Ensure that primelist contains all primes up to at least 'limit'. */
static void extend_primes_upto(uint32_t limit)
{
    if (prime_candidate < 2)
        prime_candidate = 2;
    while (prime_candidate <= limit) {
        int is_prime = 1;
        uint32_t root = (uint32_t)isqrt_uint64(prime_candidate);
        for (size_t i = 0; i < primelist_cnt; ++i) {
            uint32_t p = primelist[i];
            if (p > root)
                break;
            if (prime_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (primelist_cap == 0) {
                primelist_cap = 1000;
                primelist = (uint32_t *)malloc(primelist_cap * sizeof(uint32_t));
            } else if (primelist_cnt == primelist_cap) {
                primelist_cap <<= 1;
                primelist = (uint32_t *)realloc(primelist, primelist_cap * sizeof(uint32_t));
            }
            primelist[primelist_cnt++] = prime_candidate;
        }
        ++prime_candidate;
    }
}

/* Cache of primes p ≡ 1 (mod 4). */
static uint32_t *p2m4_list = NULL;
static size_t p2m4_cnt = 0;
static size_t p2m4_cap = 0;

/* Ensure p2m4_list contains at least 'need' such primes. */
static void extend_p2m4_up_to(size_t need)
{
    if (need <= p2m4_cnt)
        return;
    uint32_t trial = 5;          /* first prime ≡1 mod4 */
    if (p2m4_cnt == 0)
        prime_candidate = 3;     /* start generating ordinary primes from 3 */

    /* keep generating ordinary primes until we have enough p≡1(4) */
    while (p2m4_cnt < need) {
        /* grow prime list if necessary */
        if (prime_candidate == 3 && primelist_cnt == 0) {
            extend_primes_upto(3);
        }
        int is_prime = 1;
        uint32_t root = (uint32_t)isqrt_uint64(prime_candidate);
        for (size_t i = 0; i < primelist_cnt; ++i) {
            uint32_t p = primelist[i];
            if (p > root) break;
            if (prime_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            /* add to general prime list */
            if (primelist_cap == 0) {
                primelist_cap = 1000;
                primelist = (uint32_t *)malloc(primelist_cap * sizeof(uint32_t));
            } else if (primelist_cnt == primelist_cap) {
                primelist_cap <<= 1;
                primelist = (uint32_t *)realloc(primelist, primelist_cap * sizeof(uint32_t));
            }
            primelist[primelist_cnt++] = prime_candidate;

            if (prime_candidate % 4 == 1) {
                if (p2m4_cap == 0) {
                    p2m4_cap = 100;
                    p2m4_list = (uint32_t *)malloc(p2m4_cap * sizeof(uint32_t));
                } else if (p2m4_cnt == p2m4_cap) {
                    p2m4_cap <<= 1;
                    p2m4_list = (uint32_t *)realloc(p2m4_list, p2m4_cap * sizeof(uint32_t));
                }
                p2m4_list[p2m4_cnt++] = prime_candidate;
            }
        }
        ++prime_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, return 0 */

    /* generate enough primes of interest */
    extend_p2m4_up_to((size_t)n);

    /* fetch the required prime */
    uint64_t p = p2m4_list[(size_t)(n - 1)];

    /* find the unique representation p = x^2 + y^2, x<y. */
    uint64_t limit = isqrt_uint64(p / 2);
    for (uint64_t x = 1; x <= limit; ++x) {
        uint64_t r = p - x * x;
        uint64_t y = isqrt_uint64(r);
        if (y * y == r && y > x) {
            /* compute (y - x - 1)/2 */
            return (seq_t)((y - x - 1) / 2);
        }
    }

    /* should never happen for a prime ≡1 (mod 4) */
    return 0;
}
