/*
 * A111657  n-th composite number appears n-th prime times.
 * https://oeis.org/A111657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k-th composite number, where k is the least integer such
 *            that Σ_{i=1}^{k} p_i ≥ n and {p_i} are the primes in increasing
 *            order
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Resize a dynamic array. */
#ifdef __cplusplus
template <class T>
static void grow_cap(size_t *cap, size_t *size, T **arr, size_t elem_size) {
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        T *tmp = mem_resize(T, *arr, (new_cap * elem_size) / sizeof(T));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}
#else
static void grow_cap(size_t *cap, size_t *size, void **arr, size_t elem_size) {
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        void *tmp = realloc(*arr, new_cap * elem_size);
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}
#endif

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* valid for n ≥ 1 only */

    /* ---------- static state ---------- */
    static uint64_t *primes     = NULL;  /* array of primes */
    static size_t   primes_cap  = 0, primes_cnt = 0;
    static seq_t   *cum         = NULL;  /* cumulative sum of primes */
    static size_t   cum_cap     = 0, cum_cnt = 0;
    static seq_t   *comps       = NULL;  /* array of composites */
    static size_t   comp_cap    = 0, comp_cnt = 0;

    static uint64_t next_prime     = 2ULL;  /* next candidate for prime */
    static uint64_t next_comp      = 4ULL;  /* next candidate for composite */

    /* ---------- ensure enough primes / sums for this n ---------- */
    while (cum_cnt == 0 || (seq_t)n > cum[cum_cnt - 1]) {
        /* find next prime */
        uint64_t p = next_prime;
        while (!is_prime_u64(p)) p++;

        /* store the prime */
        grow_cap(&primes_cap, &primes_cnt, OEISC_VPP(&primes), sizeof(uint64_t));
        primes[primes_cnt++] = p;

        /* update cumulative sum */
        seq_t prev_sum = (cum_cnt == 0) ? (seq_t)0 : cum[cum_cnt - 1];
        grow_cap(&cum_cap, &cum_cnt, OEISC_VPP(&cum), sizeof(seq_t));
        cum[cum_cnt++] = prev_sum + (seq_t)p;

        /* advance to the following number for the next search */
        next_prime = p + 1;
    }

    /* k is the number of primes produced so far (1‑based index) */
    idx_t k = cum_cnt;

    /* ---------- ensure enough composites ---------- */
    while (comp_cnt < (size_t)k) {
        uint64_t c = next_comp;
        while (is_prime_u64(c)) c++;          /* skip primes */

        /* store the composite */
        grow_cap(&comp_cap, &comp_cnt, OEISC_VPP(&comps), sizeof(seq_t));
        comps[comp_cnt++] = (seq_t)c;

        /* next candidate starts just after this composite */
        next_comp = c + 1;
    }

    /* ---------- return the k‑th composite ---------- */
    return comps[k - 1];
}
