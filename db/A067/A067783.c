/*
 * A067783  Numbers k such that prime(k+1)^2 == prime(k)^2 (mod k).
 * https://oeis.org/A067783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that prime(k+1)^2 ≡ prime(k)^2 (mod k). The
 *            sequence is defined by scanning k = 1,2,3,… and collecting those
 *            k that satisfy the congruence. The function stores the
 *            qualifying k's in a static cache so that subsequently requested
 *            indices can be answered in O(1). This implementation wo...
 * Range:     u8 n <= 17, u16 n <= 42, u32 n <= 44, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Cache of found k values.  cache[i] is the (i+1)-th term. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next k value to test.  We start at 1 (offset 1). */
static idx_t next_k = 1;

/* Ensure cache capacity for at least one more element. */
static void
ensure_cap(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                 /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t k  = next_k;
        uint32_t pk  = get_prime(k);     /* prime(k)   */
        uint32_t pk1 = get_prime(k+1);   /* prime(k+1) */

        seq_t mod = (seq_t)k;

        /* Compute (prime(k)^2) mod k and (prime(k+1)^2) mod k. */
        seq_t pkm = (seq_t)(pk  % (uint64_t)k);
        seq_t pk1m= (seq_t)(pk1 % (uint64_t)k);
        seq_t pk_mod  = (pkm  * pkm)  % mod;
        seq_t pk1_mod = (pk1m * pk1m) % mod;

        if (pk_mod == pk1_mod) {
            ensure_cap();
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
