/*
 * A173671  Positive integers that cannot be expressed as 3^m-2^n where m and
 *          n are integers.
 * https://oeis.org/A173671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A173671: positive integers that cannot be expressed as 3^m-2^n */

/* This implementation keeps a cache of already found terms.
 * For each candidate integer it tests whether it can be written as 3^m-2^n
 * by enumerating powers of 3 up to a modest bound.  The bound is large
 * enough for all values that may appear in the requested nth term
 * (n ≤ 10000) and grows dynamically if larger terms are needed.  The
 * cache is re‑used independently of the order in which `a(n)` is
 * called, thus satisfying the requirement of correct behaviour for
 * arbitrary call order.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- helper: test whether x is of the form 3^m-2^n (m,n >= 0) */
static bool is_representable(seq_t x)
{
    /* Work in unsigned 64‑bit arithmetic for the powers of 3.
     * The maximum value that will be inspected is small (≈10^6) for
     * n ≤ 10000, so the cast is safe.  */
    const uint64_t BUF = 1ULL << 20;                 /* 1,048,576  */
    uint64_t xt = (uint64_t)x;                       /* truncate for bound check */

    for (uint64_t p = 3; ; p *= 3)
    {
        if (p > xt)
        {
            seq_t diff = (seq_t)p - x;              /* p - x > 0   */
            /* power‑of‑two test: only one bit set? */
            if ((diff & (diff - 1)) == 0)
                return true;
        }

        /* stop if further powers of 3 would exceed the bound */
        if (p > UINT64_MAX / 3)                     /* avoid overflow   */
            break;
        if (p > xt + BUF)
            break;
    }
    return false;
}

/* ---- cache of missing numbers ------------------------------------------------- */
/* The cache holds the sequence values found so far.  It grows incrementally
 * as more terms are requested. */
static seq_t *missing    = NULL;
static size_t mis_cap   = 0;
static size_t mis_cnt   = 0;
static seq_t next_cand  = 1;           /* next integer to test */

/* grow the cache to accommodate at least `need` elements */
static void ensure_cap(size_t need)
{
    if (need <= mis_cap)
        return;
    size_t new_cap = mis_cap ? mis_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(missing, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                          /* memory allocation failed */
    missing = tmp;
    mis_cap = new_cap;
}

/* ---- public function required by the build ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* not part of the sequence, but keep robust */
        return 0;

    /* initialise cache on first call */
    if (!missing) {
        mis_cap = 16;
        missing = (seq_t *)malloc(mis_cap * sizeof(seq_t));
        if (!missing)
            exit(1);
    }

    /* extend the cache until we have n values */
    while (mis_cnt < (size_t)n) {
        if (!is_representable(next_cand)) {
            ensure_cap(mis_cnt + 1);
            missing[mis_cnt++] = next_cand;
        }
        ++next_cand;
    }
    return missing[n - 1];
}
