/*
 * A022438  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c(n-1) + c(n-3) where c is the complementary sequence of
 *            a. Initial values: a(0)=2, a(1)=3, a(2)=5. The function is
 *            memoised: any n ≥ 0 can be requested in any order
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
# include <new>
#endif

/* ----- static data ----------------------------------------------------- */
static seq_t *a_vals = NULL; /* array containing the a‑sequence */
static idx_t  a_len  = 0;     /* number of elements stored in a_vals   */
static idx_t  a_cap  = 0;     /* allocated capacity in a_vals           */

static seq_t *b_vals = NULL; /* array containing the complementary b‑sequence */
static idx_t  b_len  = 0;     /* number of elements stored in b_vals   */
static idx_t  b_cap  = 0;     /* allocated capacity in b_vals           */

/* next integer to examine when building the complementary sequence */
static idx_t next_candidate = 1;

/* ----- helper to grow a_vals ------------------------------------------ */
static void ensure_a_cap(idx_t need)
{
    if (a_cap >= need) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_vals = tmp;
#ifdef __cplusplus
    /* default‑construct any newly added objects (C++ only) */
    for (idx_t i = a_cap; i < new_cap; ++i)
        new (&a_vals[i]) seq_t();
#endif
    a_cap = new_cap;
}

/* ----- helper to grow b_vals ------------------------------------------ */
static void ensure_b_cap(idx_t need)
{
    if (b_cap >= need) return;
    idx_t new_cap = b_cap ? b_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t*)realloc(b_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    b_vals = tmp;
#ifdef __cplusplus
    for (idx_t i = b_cap; i < new_cap; ++i)
        new (&b_vals[i]) seq_t();
#endif
    b_cap = new_cap;
}

/* ----- binary search to test if a value belongs to the a‑sequence ----- */
static bool in_a(idx_t val)
{
    idx_t lo = 0, hi = a_len;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t mid_val = a_vals[mid];
        if ((seq_t)val < mid_val)
            hi = mid;
        else if ((seq_t)val > mid_val)
            lo = mid + 1;
        else
            return true;
    }
    return false;
}

/* ----- generate b values up to the required index --------------------- */
static void generate_b_up_to(idx_t need)
{
    while (b_len <= need) {
        /* skip integers that are already in a */
        while (in_a(next_candidate))
            ++next_candidate;
        /* add the next complementary number */
        ensure_b_cap(b_len + 1);
        b_vals[b_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ----- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (a_vals == NULL) {
        ensure_a_cap(3);              /* reserve space for the 3 base values */
        a_vals[0] = (seq_t)2;
        a_vals[1] = (seq_t)3;
        a_vals[2] = (seq_t)5;
        a_len = 3;
    }

    /* compute all values up to the requested index */
    while (a_len <= n) {
        idx_t i   = a_len;          /* index to compute next */
        idx_t idx1 = i - 1;
        idx_t idx3 = i - 3;

        /* make sure the needed complementary values exist */
        if ((size_t)idx1 >= b_len) generate_b_up_to(idx1);
        if ((size_t)idx3 >= b_len) generate_b_up_to(idx3);

        /* recurrence: a(i) = b(i-1) + b(i-3) */
        seq_t val = b_vals[idx1] + b_vals[idx3];

        ensure_a_cap(a_len + 1);
        a_vals[a_len++] = val;
    }

    return a_vals[n];
}
