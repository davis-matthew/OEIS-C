/*
 * A072413  Numbers k such that the LCM of exponents in the prime
 *          factorization of k does not equal the product of the exponents.
 * https://oeis.org/A072413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072413 – Numbers k such that the LCM of exponents in the prime factorisation of k
   does not equal the product of the exponents.
   This implementation enumerates k in increasing order and caches the
   results to answer arbitrary queries a(n) in O(1) after the
   initial pass.  The density is about 3.6 %, so for n≤10000 the
   largest k is below 3⋅10^5, which is easily factored by trial
   division using a pre‑computed prime list.  The algorithm uses only
   integer arithmetic and works for 8,16,32,64‑bit and arbitrary‑
   precision builds. */

/* ---------- utilities ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* prime list: all primes ≤ 2000 (sufficient for n≤10000) */
#define PRIME_MAX 2000
static uint32_t primes[PRIME_MAX + 1];
static size_t primes_cnt = 0;
static bool primes_initialised = false;
static void init_primes(void)
{
    if (primes_initialised) return;
    bool is_prime[PRIME_MAX + 1];
    for (size_t i = 0; i <= PRIME_MAX; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= PRIME_MAX; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= PRIME_MAX; q += p)
                is_prime[q] = false;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
    primes_initialised = true;
}

/* simple GCD for unsigned 64‑bit values */

/* ---------- caching of sequence values ---------- */
static seq_t *term_cache = NULL;   /* dynamic array of a(i) */
static size_t term_cnt = 0;        /* number of elements stored */
static size_t term_cap = 0;        /* allocated capacity */
static seq_t last_k = 1;           /* last candidate checked */

/* enlarge cache capacity */
static void grow_term_cache(size_t min_cap)
{
    if (term_cap >= min_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 128;
    if (new_cap < min_cap) new_cap = min_cap;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cap = new_cap;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* assure primes are ready */
    init_primes();

    /* grow cache until we have at least n terms */
    while (term_cnt < (size_t)n)
    {
        /* find next candidate k */
        ++last_k;
        uint64_t k = (uint64_t)last_k;

        /* factorise k, collecting exponents up to 32 distinct primes */
        uint32_t exps[32];
        size_t exps_cnt = 0;
        uint64_t temp = k;

        for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= temp; ++pi)
        {
            uint32_t p = primes[pi];
            if (temp % p == 0)
            {
                uint32_t e = 0;
                while (temp % p == 0)
                {
                    temp /= p;
                    ++e;
                }
                if (exps_cnt < 32) exps[exps_cnt++] = e;
            }
        }
        if (temp > 1)
        {
            if (exps_cnt < 32) exps[exps_cnt++] = 1;   /* remaining prime factor */
        }

        /* compute product and LCM of exponents in 128‑bit for safety */
        unsigned __int128 prod128 = 1;
        unsigned __int128 lcm128 = 1;
        for (size_t i = 0; i < exps_cnt; ++i)
        {
            uint64_t e = exps[i];
            prod128 *= e;
            uint64_t g = gcd64((uint64_t)lcm128, e);
            lcm128 = (lcm128 / g) * e;
        }

        /* if product differs from LCM, k belongs to the sequence */
        if (prod128 != lcm128)
        {
            grow_term_cache(term_cnt + 1);
            term_cache[term_cnt++] = last_k;
        }
    }

    /* return the requested element (1‑based index) */
    return term_cache[(size_t)n - 1];
}
