/*
 * A117385  Positive integers, each occurring twice in the sequence, such that
 *          a(n) = a(k) when n+k = 5*a(n), starting with a(1)=1 and filling
 *          the next vacant position with the smallest unused number.
 * https://oeis.org/A117385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A117385 :  a(n) = a(5*a(n)-n)  (OEIS offset 1)
    Each positive integer occurs exactly twice, the two occurrences
    being at indices n and k = 5*a(n)-n.  The sequence is formed by
    scanning positions in increasing order; when an empty position is
    found, the smallest unused integer is placed there and its partner
    index is computed immediately.  All values are memoised in a
    static array, so any call to a(n) in any order works in O(n).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static seq_t *A = NULL;          /* a(1), a(2), …   */
static size_t A_cap = 0;         /* capacity of A   */
static idx_t  last_filled = 0;   /* highest n processed */

static bool *used = NULL;        /* used[i] true if i already used */
static size_t used_cap = 0;      /* capacity of used  */
static idx_t next_candidate = 1; /* next integer to try */

static inline void ensure_A(size_t n)
{
    if (A_cap >= n) return;
    size_t new_cap = (A_cap == 0) ? 1 : A_cap * 2;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memset(tmp + A_cap, 0, (new_cap - A_cap) * sizeof(seq_t));
    A = tmp;
    A_cap = new_cap;
}

static inline void ensure_used(size_t n)
{
    if (used_cap > n) return;
    size_t new_cap = (used_cap == 0) ? 1 : used_cap * 2;
    if (new_cap <= n) new_cap = n + 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but avoid crash */

    /* ensure array can hold index n */
    ensure_A((size_t)n);

    /* process all yet-unfilled positions up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        if (A[(size_t)(i-1)] != 0) continue;  /* already set by partner */

        /* find smallest unused positive integer */
        idx_t m = next_candidate;
        while ((size_t)m < used_cap && used[(size_t)m]) ++m;
        ensure_used((size_t)m);
        /* if m is still used after resizing (rare) skip used values */
        while (used[(size_t)m]) ++m;
        ensure_used((size_t)m);

        used[(size_t)m] = true;
        next_candidate = m + 1;

        A[(size_t)(i-1)] = (seq_t)m;

        /* partner index k = 5*m - i */
        idx_t k = 5 * m - i;
        if (k == 0) continue;  /* should not happen for valid n */

        ensure_A((size_t)k);
        if (A[(size_t)(k-1)] == 0) {
            A[(size_t)(k-1)] = (seq_t)m;
        }
    }

    last_filled = n;
    return A[(size_t)(n-1)];
}
