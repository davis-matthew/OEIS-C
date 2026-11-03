/*
 * A174620  a(n) = 2^A141468(n) mod prime(n), where A141468(n) is the n-th
 *          nonnegative nonprime.
 * https://oeis.org/A174620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A141468(n) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 6600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9847 (2026-11-03)
 * Helpers:   is_prime_seq, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
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

static seq_t *nonprimes = NULL;   /* cache of non‑prime integers (0,1,4,6,…) */
static size_t np_cnt   = 0;       /* number of cached non‑primes          */
static size_t np_cap   = 0;       /* current capacity of the cache        */
static idx_t  last_seen = 0;      /* next integer to test for non‑prime   */

static void grow_nonprimes(void)
{
    size_t new_cap = np_cap == 0 ? 128 : np_cap << 1;
    seq_t *tmp = mem_resize(seq_t, nonprimes, new_cap);
    if (!tmp) abort();           /* cannot allocate more memory  */
    nonprimes = tmp;
    np_cap    = new_cap;
}

/* Fill the cache with at least n non‑prime values. */
static void ensure_nonprimes(idx_t n)
{
    while (np_cnt < (size_t)n) {
        seq_t val = (seq_t)last_seen;
        if (!is_prime_seq(val)) {
            if (np_cnt == np_cap) grow_nonprimes();
            nonprimes[np_cnt++] = val;
        }
        ++last_seen;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* safety guard – OEIS offset is 1 */

    /* (1)  Get the n‑th non‑prime (A141468). */
    ensure_nonprimes(n);
    seq_t np_val = nonprimes[(size_t)(n - 1)]; /* 0‑based array   */

    /* (2)  Get the n‑th prime. */
    uint32_t p32 = get_prime(n);               /* prime fits in 32 bits  */
    seq_t prime  = (seq_t)p32;                 /* cast to seq_t           */

    /* (3)  Compute 2^np_val  (mod prime). */
    return powmod((seq_t)2, (uint64_t)np_val, prime);
}
