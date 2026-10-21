/*
 * A078175  Numbers with an integer arithmetic mean of all prime factors.
 * https://oeis.org/A078175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with an integer arithmetic mean of all prime factors.
   a(n) = the n‑th smallest integer m (m ≥ 2) such that
          (sum of the prime factors of m, counted with multiplicity)
/          (number of prime factors of m) is an integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- prime list helpers ------------------------------------------ */
static uint32_t *prime_list = NULL;      /* array of primes */
static size_t prime_cap   = 0;           /* allocated size of prime_list */
static size_t prime_cnt   = 0;           /* number of primes stored   */
static uint32_t last_checked = 1;        /* largest integer tested for primality */

/* enlarge the prime array when needed */
static void grow_primes(size_t min_cap) {
    if (min_cap <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* generate primes up to 'limit' (inclusive) */
static void ensure_primes_upto(uint32_t limit) {
    if (limit <= last_checked) return;

    for (uint32_t n = last_checked + 1; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if (p > n / p) break;          /* p*p > n */
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes(prime_cnt + 1);
            prime_list[prime_cnt++] = n;
        }
    }
    last_checked = limit;
}

/* ---- main sequence generator ------------------------------------- */
seq_t a(idx_t n)
{
    /* caches of already computed terms */
    static seq_t *cache    = NULL;
    static idx_t cached    = 0;      /* number of terms stored   */
    static idx_t cache_cap = 0;      /* allocated capacity       */

    static uint64_t next = 2;        /* next integer to test    */

    /* make sure we have at least n terms cached */
    while (cached < n) {
        uint64_t m = next;
        seq_t sum = 0;               /* sum of prime factors   */
        idx_t  cnt = 0;               /* number of prime factors with multiplicity */

        /* factor 2 separately for speed */
        while ((m & 1ULL) == 0) {
            m >>= 1;
            sum += 2;
            cnt++;
        }

        /* prepare list of primes up to √m */
        uint32_t limit = (uint32_t)(m > 0 ? (uint32_t)(m / m) : 0); /* dummy */
        /* compute integer sqrt of m */
        uint64_t r = 1;
        while (r * r <= m) r++;
        r--; /* now r = floor(sqrt(m)) */
        if (r > 0 && r > 1u) limit = (uint32_t)r;
        else limit = 2;

        ensure_primes_upto(limit);

        /* trial division by known primes */
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] <= m / prime_list[i]; ++i) {
            uint32_t p = prime_list[i];
            while (m % p == 0) {
                m /= p;
                sum += p;
                cnt++;
            }
        }

        /* any remaining prime factor? */
        if (m > 1) {
            sum += m;
            cnt++;
        }

        /* check the integer mean condition */
        if (cnt > 0 && sum % (seq_t)cnt == 0) {
            /* store the term */
            if (cached == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cached++] = (seq_t)next;
        }
        next++; /* move on to next integer            */
    }

    return cache[n - 1];
}
