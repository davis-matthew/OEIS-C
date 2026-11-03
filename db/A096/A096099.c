/*
 * A096099  a(0) = 1, a(n) = least number such that the concatenation of all
 *          terms through a(n) is divisible by prime(n).
 * https://oeis.org/A096099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the least integer m ≥ 1 such that the concatenation
 *            a(0)...a(n) is divisible by the n‑th prime (prime(0)=2). The
 *            following implementation computes a(n) on demand with
 *            memoisation
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 954 (2026-11-03)
 * Helpers:   mem, num_digits, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <stddef.h>
#include "../utils/num_digits.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

static void free_cache(seq_t *p)
{
    mem_free(p);
}

/* Cache of computed terms.  cache[i]==0 indicates "not yet computed". */
static seq_t *cache   = NULL;
static size_t cache_len = 0;

/* Allocate (or enlarge) the cache to hold indices 0..n.  All new
   entries are initialised to 0. */
static void ensure_cache(size_t n)
{
    if (cache_len > n) return;

    size_t new_len = (cache_len == 0) ? 1 : cache_len;
    while (new_len <= n) new_len <<= 1;

    /* Allocate new array */
    seq_t *new_cache = alloc_cache((idx_t)new_len);
    /* Initialise new slots to zero */
    for (size_t i = 0; i < new_len; ++i) new_cache[i] = 0;

    /* Copy old values and free the old cache */
    if (cache) {
        size_t copy_len = (cache_len < new_len) ? cache_len : new_len;
        for (size_t i = 0; i < copy_len; ++i) new_cache[i] = cache[i];
        free_cache(cache);
    }

    cache   = new_cache;
    cache_len = new_len;
}

/* Recursive helper that guarantees that cache[n] is filled. */
static seq_t compute_a(idx_t n)
{
    if (n == 0) {               /* base case */
        cache[0] = 1;
        return 1;
    }

    if (cache[n] != 0) return cache[n];     /* already computed */

    /* Ensure all previous terms are available */
    compute_a(n - 1);

    /* Prime for this index */
    uint32_t p32 = get_prime(n);
    seq_t  p    = (seq_t)p32;                /* cast to seq_t */

    /* R = remainder of concatenation a(0)...a(n-1) modulo p */
    seq_t R = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t ai = cache[i];
        int    di = num_digits(ai);               /* decimal digits of ai */
        seq_t pow10 = powmod((seq_t)10, (idx_t)di, p);
        seq_t ai_mod = ai % p;
        R = (R * pow10 + ai_mod) % p;
    }

    /* Find the smallest k ≥ 1 with (R * 10^len(k) + k) ≡ 0 (mod p) */
    seq_t k = 1;
    for (;;) {
        int dk = num_digits(k);
        seq_t pow10k = powmod((seq_t)10, (idx_t)dk, p);
        seq_t test = (R * pow10k + k) % p;
        if (test == 0) break;
        ++k;
    }

    cache[n] = k;
    return k;
}

/* Public API */
seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return compute_a(n);
}
