/*
 * A182404  Numbers whose digit sum as well as sum of the squares of the
 *          digits is a prime.
 * https://oeis.org/A182404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   get_digits, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer whose digit sum
 *               and sum of the squares of its digits are both prime.
 *               (OEIS A182404)
 *
 * The function generates the sequence lazily, caching values between
 * calls.  It works for any integer type chosen for seq_t and idx_t.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* External function signature required by the harness */
seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Current candidate being tested */
    static seq_t cur = (seq_t)1;

    /* Ensure we have enough space for at least one more term */
    if (cache_len == 0) {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least n terms */
    while ((idx_t)cache_len < n) {
        /* Compute digit sum and digit-square sum of cur */
        uint64_t dsum = 0, dsq = 0;
        size_t dlen = 0;
        /* Buffer large enough for most numbers encountered */
        uint8_t digs[2048];

        get_digits(cur, digs, &dlen);
        for (size_t i = 0; i < dlen; ++i) {
            uint8_t d = digs[i];
            dsum += d;
            dsq += (uint64_t)d * (uint64_t)d;
        }

        /* Test primeness of sums */
        if (is_prime_seq(dsum) && is_prime_seq(dsq)) {
            /* Store cur into cache */
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = cur;
        }

        /* Advance to next candidate */
        ++cur;
    }

    /* Return the (n-1)-th cached term (a(1) is cache[0]) */
    return cache[n - 1];
}
