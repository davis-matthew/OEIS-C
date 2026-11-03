/*
 * A066875  Numbers k such that prime(k+1) + prime(k-1) = 2*prime(k).
 * https://oeis.org/A066875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 992 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066875: indices k such that prime(k-1) + prime(k+1) = 2·prime(k)
    (balanced prime indices).  The function builds the indices on
    demand, caching them for subsequent calls.  It uses the supplied
    utilities get_prime, alloc_seq_array and seq_free_array. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *bal_indices   = NULL;   /* cached balanced prime indices */
static size_t bal_len        = 0;      /* number of indices cached  */
static size_t bal_cap        = 0;      /* current capacity of bal_indices */
static idx_t  next_k         = 3;      /* next prime index to test          */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* Expand cached list until we have at least n terms. */
    while (bal_len < (size_t)n) {
        /* Fetch neighbouring primes. */
        uint64_t pk_prev = get_prime(next_k - 1);
        uint64_t pk_cur  = get_prime(next_k);
        uint64_t pk_next = get_prime(next_k + 1);

        /* Balanced prime test. */
        if (pk_prev + pk_next == 2ULL * pk_cur) {
            /* Grow the storage if necessary. */
            if (bal_len + 1 > bal_cap) {
                size_t new_cap = bal_cap ? bal_cap * 2 : 16;
                seq_t *new_arr = alloc_seq_array(new_cap);
                if (!new_arr) return 0;                 /* allocation failure */
                for (size_t i = 0; i < bal_len; ++i)
                    new_arr[i] = bal_indices[i];
                if (bal_indices) seq_free_array(bal_indices);
                bal_indices = new_arr;
                bal_cap = new_cap;
            }
            bal_indices[bal_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return bal_indices[n - 1];
}
