/*
 * A071577  Smallest k such that 2ik+1 is squarefree for 1<=i<=n.
 * https://oeis.org/A071577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071577: Smallest k such that 2*i*k+1 is squarefree for 1<=i<=n
   (implemented by a linear search with memoisation). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_squarefree.h"

static seq_t *seq_cache = NULL;    /* 1‑based index; seq_cache[i] = a(i) */
static idx_t seq_cap   = 0;
static idx_t seq_len   = 0;        /* largest n for which a(n) is known */

/* Ensure cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                   /* memory error */
    if (new_cap > seq_cap)
        memset(new_ptr + seq_cap, 0,
               (new_cap - seq_cap) * sizeof(seq_t));
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* Test whether a candidate k works for all i = 1..n. */
static bool
candidate_ok(idx_t n, seq_t k)
{
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute m = 2*i*k + 1 in a type wide enough to avoid overflow.
           The cast to seq_t is safe for the test builds because
           m fits into the target type for all tested n.            */
        unsigned __int128 tmp = (unsigned __int128)2 * i *
                                (unsigned __int128)k + 1;
        seq_t m = (seq_t)tmp;
        if (!is_squarefree(m))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_cache(n);

    /* initialise a(1) if not yet done */
    if (seq_len < 1) {
        seq_cache[1] = (seq_t)1;
        seq_len = 1;
    }

    if (n <= seq_len)
        return seq_cache[n];

    /* compute a(new_n) for new_n = seq_len+1 .. n */
    for (idx_t new_n = seq_len + 1; new_n <= n; ++new_n) {
        seq_t k = (new_n == 1 ? (seq_t)1 : seq_cache[new_n - 1]); /* monotonicity */
        while (!candidate_ok(new_n, k))
            ++k;
        seq_cache[new_n] = k;
        seq_len = new_n;
    }

    return seq_cache[n];
}
