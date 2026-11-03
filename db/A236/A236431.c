/*
 * A236431  Numbers n such that both prime(n)+n and prime(n)-n give a
 *          triangular number.
 * https://oeis.org/A236431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) = nth index i such that prime(i)+i and prime(i)-i are
 *            triangular numbers
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *indices = NULL;          /* stored indices that satisfy the condition */
static size_t idx_len = 0;             /* number of stored indices */
static size_t idx_cap = 0;             /* allocated capacity of indices */
static idx_t last_checked = 1;         /* next n to check for the condition */

/* test whether a number is triangular: 8*x+1 must be a perfect square */
static bool is_triangular(seq_t x)
{
    seq_t v = x * 8 + 1;
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* append a new index to the dynamic array, growing it if necessary */
static void append_index(seq_t val)
{
    if (idx_len == idx_cap) {
        idx_cap = idx_cap == 0 ? 64 : idx_cap * 2;
        seq_t *tmp = mem_resize(seq_t, indices, idx_cap);
        if (tmp == NULL) /* allocation failure – terminate program */
            abort();
        indices = tmp;
    }
    indices[idx_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; but guard against misuse */

    /* Fill the indices array until it contains at least n elements */
    while (idx_len < n) {
        seq_t p = get_prime(last_checked);        /* prime at position last_checked */
        seq_t plus  = p + last_checked;
        seq_t minus = p - last_checked;
        if (is_triangular(plus) && is_triangular(minus))
            append_index(last_checked);
        last_checked++;
    }

    return indices[n - 1];
}
