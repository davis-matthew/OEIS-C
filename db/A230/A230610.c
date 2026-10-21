/*
 * A230610  Number of nX4 0..2 arrays x(i,j) with each element horizontally or
 *          antidiagonally next to at least one element with value (x(i,j)+1)
 *          mod 3, and upper left element zero.
 * https://oeis.org/A230610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -45*a(n-2) +42*a(n-3) -12*a(n-4) + a(n-5)
 *            initial values: a(1)=0, a(2)=2, a(3)=30, a(4)=348, a(5)=3956
 *            Recurrence applies for n ≥ 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------
   Memory management for the C implementation (seq_t is assumed to be
   trivially movable).  The C++ implementation uses std::vector, which
   handles construction/destruction of arbitrary‑precision classes.
   -------------------------------------------------------------------- */

#ifndef __cplusplus
/* C version ---------------------------------------------------------- */
static seq_t *cache_c = NULL;      /* array of cached values */
static size_t cache_len_c = 0;      /* number of values stored */
static size_t cache_cap_c = 0;      /* allocated capacity */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_capacity_c(size_t need)
{
    if (need <= cache_cap_c)
        return;
    size_t new_cap = cache_cap_c ? cache_cap_c * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache_c, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* Out of memory */
    cache_c    = tmp;
    cache_cap_c = new_cap;
}
#endif

/* --------------------------------------------------------------------
   a() implementation
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                  /* defensive: OEIS offset starts at 1 */
        return (seq_t)0;

    /* C++ implementation --------------------------------------------- */
#ifdef __cplusplus
    static std::vector<seq_t> cache_cpp;

    /* Grow cache to contain a(n) (index n-1) */
    size_t target = (size_t)n;   /* number of terms needed */
    if (cache_cpp.size() < target) {
        size_t old = cache_cpp.size();
        cache_cpp.reserve(target);

        for (size_t k = old; k < target; ++k) {   /* k is 0‑based index */
            seq_t val;
            if (k == 0)
                val = (seq_t)0;
            else if (k == 1)
                val = (seq_t)2;
            else if (k == 2)
                val = (seq_t)30;
            else if (k == 3)
                val = (seq_t)348;
            else if (k == 4)
                val = (seq_t)3956;
            else {
                /* recurrence: a(k+1) = 15*a(k) -45*a(k-1) +42*a(k-2)
                               -12*a(k-3) + a(k-4) */
                seq_t positive = cache_cpp[k-1] * (seq_t)15
                               + cache_cpp[k-3] * (seq_t)42
                               + cache_cpp[k-5];
                seq_t negative = cache_cpp[k-2] * (seq_t)45
                               + cache_cpp[k-4] * (seq_t)12;
                val = positive - negative;          /* final subtraction */
            }
            cache_cpp.push_back(val);
        }
    }
    return cache_cpp[n-1];

#else   /* C implementation ------------------------------------------- */
    size_t target = (size_t)n;   /* number of terms needed */
    if (cache_len_c < target) {
        ensure_capacity_c(target);
        for (size_t k = cache_len_c; k < target; ++k) {   /* k is 0‑based index */
            seq_t val;
            if (k == 0)
                val = (seq_t)0;
            else if (k == 1)
                val = (seq_t)2;
            else if (k == 2)
                val = (seq_t)30;
            else if (k == 3)
                val = (seq_t)348;
            else if (k == 4)
                val = (seq_t)3956;
            else {
                seq_t positive = cache_c[k-1] * (seq_t)15
                               + cache_c[k-3] * (seq_t)42
                               + cache_c[k-5];
                seq_t negative = cache_c[k-2] * (seq_t)45
                               + cache_c[k-4] * (seq_t)12;
                val = positive - negative;
            }
            cache_c[k] = val;
        }
        cache_len_c = target;
    }
    return cache_c[n-1];

#endif
}
