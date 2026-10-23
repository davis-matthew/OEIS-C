/*
 * A125639  Doubly abundant numbers - numbers k such that k and s(k) are
 *          abundant, where s() is A001065.
 * https://oeis.org/A125639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that k and s(k) are abundant numbers (where s(k)
 *            is the sum of proper divisors of k)
 * Range:     u8 n <= 8, u16 n <= 2516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* forward declarations of helpers for allocation that work in both C and C++ */
static seq_t* alloc_array(size_t n);
static void free_array(seq_t* p);
static seq_t* realloc_array(seq_t* old, size_t old_elements, size_t new_cap);

/* sigma[n] = sum of all divisors of n (including 1 and n) */
static seq_t *sigma = NULL;
static size_t sigma_limit = 0;   /* number of elements in sigma (<= sigma_limit) */

/* list of doubly abundant numbers found so far */
static seq_t *doubly = NULL;
static size_t doubly_len = 0;    /* current number of stored terms */
static size_t doubly_cap = 0;    /* capacity of doubly array */

/* the next integer that will be examined */
static idx_t last_checked = 23;  /* 23 has just been checked, 24 will be next */

/* ---------- allocation helpers --------------------------------------- */
static seq_t* alloc_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();          /* value‑initialised */
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

static void free_array(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static seq_t* realloc_array(seq_t* old, size_t old_elements, size_t new_cap)
{
#ifdef __cplusplus
    seq_t* new_arr = new seq_t[new_cap]();   /* value‑initialised */
    for (size_t i = 0; i < old_elements; ++i) new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* ---------- sigma array management ------------------------------------ */
static void recompute_sigma(size_t limit)
{
    seq_t* new_arr = alloc_array(limit);
    if (!new_arr) return;   /* allocation failed – callers will crash */

    if (sigma) free_array(sigma);
    sigma = new_arr;
    /* compute divisor sums (O(limit log limit)) */
    for (size_t i = 1; i < limit; ++i)
        for (size_t j = i; j < limit; j += i)
            sigma[j] += (seq_t)i;
    sigma_limit = limit;
}

static void ensure_sigma(size_t n)
{
    if (n <= sigma_limit) return;
    size_t new_limit = sigma_limit ? sigma_limit * 2 + 10 : 64;
    while (new_limit <= n) new_limit = new_limit * 2 + 10;
    recompute_sigma(new_limit);
}

/* ---------- doubly abundant list management -------------------------- */
static void ensure_doubly_capacity(size_t needed)
{
    if (needed <= doubly_cap) return;
    size_t new_cap = doubly_cap ? doubly_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    doubly = realloc_array(doubly, doubly_len, new_cap);
    doubly_cap = new_cap;
}

/* ---------- main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    /* generate terms until we have at least n of them */
    while (doubly_len < (size_t)n) {
        ++last_checked;     /* candidate integer */

        /* ensure sigma for candidate and its s(k) */
        ensure_sigma((size_t)last_checked + 1u);
        seq_t s_k = sigma[(size_t)last_checked] - (seq_t)last_checked;

        /* k is abundant when s(k) > k */
        if (s_k > (seq_t)last_checked) {
            ensure_sigma((size_t)s_k + 1u);
            seq_t sigma_s = sigma[(size_t)s_k];
            seq_t s_s = sigma_s - (seq_t)s_k;
            if (s_s > s_k) {                 /* s(k) is also abundant */
                ensure_doubly_capacity(doubly_len + 1);
                doubly[doubly_len++] = (seq_t)last_checked;
            }
        }
    }
    return doubly[(size_t)n - 1];
}
