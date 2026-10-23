/*
 * A142850  Primes congruent to 52 mod 61.
 * https://oeis.org/A142850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 52 modulo 61.
   Returns the n‑th such prime (a(1)=113).  The function builds a
   cache of the required primes by sieving up to the smallest bound
   that contains at least n primes with p ≡ 52 (mod 61). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Allocate and free seq_t arrays with proper constructors/destructors.
   In C++ mode new/delete are used, in C mode malloc/free. */
#ifdef __cplusplus
#define ALLOC_SEQ_ARRAY(n) new seq_t[(n)]
#define FREE_SEQ_ARRAY(p) delete[] (p)
#else
#define ALLOC_SEQ_ARRAY(n) ((seq_t*)malloc((n) * sizeof(seq_t)))
#define FREE_SEQ_ARRAY(p) free((p))
#endif

/* Cache of primes ≡ 52 (mod 61). */
static seq_t *prime_cache = NULL;   /* array of primes */
static size_t cache_len  = 0;       /* number of primes cached */

/* Expand the cache to contain at least n primes of the desired form.
   The sieve is regenerated if a larger limit is required. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* already enough */

    /* Initial estimate: 700 * n (good for n up to ∼10 000). */
    uint64_t limit = (uint64_t)n * 700 + 1000;
    if (limit < 1000) limit = 1000;

    while (true) {
        /* Allocate array for the sieve (1 byte per number). */
        uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        if (!is_prime) return;                    /* out of memory – keep old cache */

        /* Mark all ≥2 as prime candidates. */
        for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = 1;

        /* Eratosthenes sieve. */
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count how many primes with p ≡ 52 (mod 61) appear. */
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (is_prime[i] && (i % 61ULL) == 52ULL) ++cnt;

        if (cnt >= n) {              /* we have enough primes – build the cache */
            seq_t *new_cache = ALLOC_SEQ_ARRAY(n);
            if (!new_cache) {         /* memory failure – keep old cache */
                FREE_SEQ_ARRAY(is_prime);
                return;
            }

            size_t idx = 0;
            for (uint64_t i = 2; i <= limit && idx < n; ++i)
                if (is_prime[i] && (i % 61ULL) == 52ULL)
                    new_cache[idx++] = (seq_t)i;

            /* Replace old cache. */
            FREE_SEQ_ARRAY(prime_cache);
            prime_cache = new_cache;
            cache_len   = n;
            FREE_SEQ_ARRAY(is_prime);
            break;
        }

        /* Not enough primes – double the limit and try again. */
        FREE_SEQ_ARRAY(is_prime);
        limit *= 2;
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0 – return 0 as placeholder */
    ensure_cache(n);
    return prime_cache[n - 1];
}
