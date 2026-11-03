/*
 * A030483  Cubes of primes, with property that all even digits occur together
 *          and all odd digits occur together.
 * https://oeis.org/A030483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030483: Cubes of primes whose cube has all even digits together
 * and all odd digits together.
 * a(n) = (A030482(n))^3
 * where A030482(n) is the nth prime whose cube satisfies the parity block
 * property. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Test whether the decimal representation of a number has the
 * property that all even digits form one contiguous block and
 * all odd digits form one contiguous block. */
static bool cube_parity_ok(seq_t value)
{
    int last_parity = -1;   /* -1 = none yet, 0 = even, 1 = odd */
    int transitions = 0;
    while (value != 0)
    {
        int digit   = (int)(value % 10);
        int parity  = digit & 1;   /* 0 for even, 1 for odd */
        if (last_parity != -1 && parity != last_parity)
        {
            ++transitions;
            if (transitions > 1) return false;
        }
        last_parity = parity;
        value /= 10;
    }
    return true;
}

/* Cached qualifying primes */
static seq_t *good_cache = NULL;
static size_t  good_cap   = 0;
static size_t  good_cnt   = 0;
/* Next prime index to examine (1‑based) */
static idx_t  prime_scan = 1;

/* Grow the dynamic array by at least needed elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

/* Expand the cache until it contains at least n qualifying primes. */
static void ensure_good_n(idx_t n)
{
    while (good_cnt < n)
    {
        seq_t p = get_prime(prime_scan);
        ++prime_scan;
        seq_t cube = p * p * p;
        if (cube_parity_ok(cube))
        {
            if (good_cnt == good_cap) grow_seq(&good_cache, &good_cap, good_cnt + 1);
            good_cache[good_cnt++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for offset 1 */
    ensure_good_n(n);
    seq_t p = good_cache[n - 1];
    return p * p * p;                  /* cube in seq_t */
}
