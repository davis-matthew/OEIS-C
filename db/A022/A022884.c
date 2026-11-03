/*
 * A022884  Numbers k such that prime(k) + prime(k+3) = prime(k+1) +
 *          prime(k+2).
 * https://oeis.org/A022884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 5824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9151 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A022884: indices k such that prime(k)+prime(k+3)=prime(k+1)+prime(k+2) */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

static idx_t *found         = NULL;   /* stored qualifying k */
static size_t found_count   = 0;       /* number stored */
static size_t found_cap     = 0;       /* allocation capacity */
static idx_t last_k         = 0;       /* last k tested */

/* Append a new qualifying k to the stored array, expanding if needed. */
static void add_found(idx_t k)
{
    if (found_count == found_cap) {
        size_t new_cap = found_cap == 0 ? 4 : found_cap * 2;
        idx_t *tmp = (idx_t *)realloc(found, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        found = tmp;
        found_cap = new_cap;
    }
    found[found_count++] = k;
}

/* External function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, guard */
    while (found_count < (size_t)n) {
        idx_t k = ++last_k;   /* next k to test */
        seq_t pk  = (seq_t)get_prime(k);
        seq_t pk1 = (seq_t)get_prime(k + 1);
        seq_t pk2 = (seq_t)get_prime(k + 2);
        seq_t pk3 = (seq_t)get_prime(k + 3);
        if (pk + pk3 == pk1 + pk2) {
            add_found(k);
        }
    }
    return (seq_t)found[n - 1];
}
