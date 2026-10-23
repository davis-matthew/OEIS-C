/*
 * A049572  Primes p such that x^40 = 2 has a solution mod p.
 * https://oeis.org/A049572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p such that 2 is a 40th power residue
 *            modulo p, i.e. 2^((p-1)/gcd(p-1,40)) ≡ 1 (mod p). The first term
 *            a(1)=2
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The routine uses only integer arithmetic; it works for seq_t of
   any size supported by the build (8–128 bits or arbitrary precision). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/powmod64.h"   /* provides uint64_t powmod64(uint64_t, uint64_t, uint64_t) */

/* --------------------------------------------------------------------- */
/* Small helpers -------------------------------------------------------- */

/* Euclidean GCD for uint64_t ------------------------------------------ */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Dynamic array growth ---------------------------------------------- */
static inline void grow(void **arr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    *arr = realloc(*arr, new_cap * elem_size);
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Prime search -------------------------------------------------------- */

static uint64_t *prime_list = NULL;      /* list of primes found so far   */
static size_t     prime_cnt  = 0;        /* number of primes stored      */
static size_t     prime_cap  = 0;        /* allocated capacity           */
static uint64_t   last_checked = 1;      /* last number examined         */

/* Trial‑division primality test ------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n & 1ULL) {               /* odd numbers only from now on */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            uint64_t pp = p * p;
            if (pp > n) break;
            if (n % p == 0) return false;
        }
        return true;
    }
    return false;
}

/* Return next prime after the previous one --------------------------- */
static uint64_t next_prime(void)
{
    /* Handle the very first prime (2) explicitly */
    if (prime_cnt == 0) {
        grow((void **)&prime_list, &prime_cap, 1, sizeof(uint64_t));
        prime_list[0] = 2ULL;
        prime_cnt = 1;
        last_checked = 2ULL;
        return 2ULL;
    }

    uint64_t candidate = last_checked + 1;
    if (candidate <= 2ULL) candidate = 2ULL;
    else if ((candidate & 1ULL) == 0) ++candidate;   /* start with an odd number */

    for (;; candidate += 2) {
        if (is_prime_uint64(candidate)) {
            grow((void **)&prime_list, &prime_cap, prime_cnt + 1, sizeof(uint64_t));
            prime_list[prime_cnt++] = candidate;
            last_checked = candidate;
            return candidate;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Good‑prime test ----------------------------------------------------- */

static bool good_prime(uint64_t p)
{
    if (p == 2ULL) return true;            /* special case: 0^40 == 2 (mod 2) */

    uint64_t g   = gcd_uint64(40ULL, p - 1ULL);
    uint64_t exp = (p - 1ULL) / g;
    return powmod64(2ULL, exp, p) == 1ULL;
}

/* --------------------------------------------------------------------- */
/* Sequence cache ------------------------------------------------------ */

static seq_t *seq_cache = NULL;     /* cached sequence values    */
static size_t seq_cnt  = 0;         /* number of cached entries  */
static size_t seq_cap  = 0;         /* capacity of seq_cache     */

/* --------------------------------------------------------------------- */
/* The required function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 0) return 0;              /* offset is 1 */

    /* Generate terms until we have the n‑th one */
    while (seq_cnt < (size_t)n) {
        uint64_t p = next_prime();
        if (good_prime(p)) {
            grow((void **)&seq_cache, &seq_cap, seq_cnt + 1, sizeof(seq_t));
            seq_cache[seq_cnt++] = (seq_t)p;
        }
    }
    return seq_cache[n - 1];
}
