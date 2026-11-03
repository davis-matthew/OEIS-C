/*
 * A068359  Binomial(2k,k) when the first digit of binomial(2k,k) is 1.
 * https://oeis.org/A068359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th central binomial coefficient C(2k,k) whose first
 *            decimal digit is 1. The offset is 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for out‑of‑range calls */

    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;

    /* current k (starting with 0) and the exact value of C(2k,k) */
    static idx_t k            = 0;
    static unsigned __int128 current128 = 1;   /* 1 for k = 0 */

    /* largest value that can be represented in seq_t */
    static seq_t max_val       = 0;
    static bool   max_val_init = false;
    if (!max_val_init) {
        for (size_t i = 0; i < sizeof(seq_t) * 8; ++i)
            max_val = (max_val << 1) + 1;
        max_val_init = true;
    }

    /* generate until we have the requested term */
    while (cache_len < (size_t)n) {
        /* if current value no longer fits in seq_t, we cannot store it */
        if (current128 > (unsigned __int128)max_val)
            break;

        seq_t current = (seq_t)current128;

        /* test if the first decimal digit is 1 */
        idx_t len  = num_digits(current);
        seq_t pow  = pow10((idx_t)(len - 1));
        seq_t fd   = current / pow;
        if (fd == (seq_t)1) {
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = current;
        }

        /* advance to the next central binomial coefficient */
        idx_t k_next = k + 1;
        unsigned __int128 factor = (unsigned __int128)(4 * k_next - 2);
        current128 = current128 * factor / (unsigned __int128)k_next;
        k = k_next;
    }

    if (cache_len < (size_t)n) return 0;  /* term too large for seq_t */
    return cache[n - 1];
}
