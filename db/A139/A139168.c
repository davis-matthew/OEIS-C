/*
 * A139168  a(n) = (prime(n)! + 10)/10.
 * https://oeis.org/A139168
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)! + 10) / 10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static inline seq_t seq_one(void) { return (seq_t)1; }

static seq_t *facts = NULL;          /* factorial of prime(1..k) stored 0‑based */
static size_t facts_cap = 0;         /* capacity of facts */
static idx_t facts_size = 0;         /* number of entries stored */
static seq_t current_factorial = 1;  /* factorial of the last processed prime */
static uint64_t last_prime_val = 0;  /* value of the last processed prime */

seq_t a(idx_t n)
{
    /* For indices below the offset, return 0; behaviour is undefined otherwise */
    if (n < 3)
        return 0;

    /* Allocate the facts array if first call or grow it enough */
    if (facts_size < n) {
        size_t new_cap = facts_cap ? facts_cap : 8;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        ensure_capacity(new_cap, &facts_cap, OEISC_VPP(&facts), sizeof(seq_t));
    }

    /* If we have already computed up to n, just use the cached factorial */
    if (facts_size >= n) {
        seq_t fac = facts[n - 1];
        return (fac + (seq_t)10) / (seq_t)10;
    }

    /* Compute factorials for the remaining primes up to n */
    for (idx_t idx = facts_size + 1; idx <= n; ++idx) {
        /* Obtain the prime for this index */
        uint32_t p = get_prime(idx);

        /* Multiply through all integers from last_prime_val+1 up to p */
        for (idx_t x = last_prime_val + 1; x <= (idx_t)p; ++x)
            current_factorial = current_factorial * (seq_t)x;

        /* Store the factorial of the current prime */
        facts[idx - 1] = current_factorial;
        last_prime_val = p;
    }

    facts_size = n;
    seq_t fac = facts[n - 1];
    return (fac + (seq_t)10) / (seq_t)10;
}
