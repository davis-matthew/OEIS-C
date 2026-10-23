/*
 * A171601  The largest part in the set of all {x,y,z} for any partitioning
 *          n=x+y+z into three triangular numbers x, y and z.
 * https://oeis.org/A171601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest part in a partition of n into three triangular numbers.
   Implements the definition of OEIS A171601 by dynamically generating the
   triangular numbers up to n, then searching for the maximal triangular
   component that participates in a partition of n = T_i + T_j + T_k
   with 0 <= i <= j <= k.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Dynamic cache of triangular numbers T_m = m*(m+1)/2                */
static seq_t *tri = NULL;   /* indexed by m,  tri[0] = 0                */
static size_t tri_len = 0;  /* number of elements currently stored    */
static size_t tri_cap = 0;  /* allocated capacity                     */

static void ensure_tri_up_to(idx_t limit)
{
    if (tri_cap == 0) {
        tri_cap = 64;
        tri = (seq_t *)malloc(tri_cap * sizeof(seq_t));
        tri_len = 1;           /* tri[0] = 0                         */
        tri[0] = 0;
    }
    /* Grow array until the next triangular number would exceed limit. */
    while (true) {
        idx_t next_m = (idx_t)tri_len;
        seq_t next_val = tri[tri_len - 1] + (seq_t)next_m; /* T_m = T_{m-1}+m */
        if (next_val > (seq_t)limit)
            break;
        if (tri_len == tri_cap) {
            tri_cap *= 2;
            tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
        }
        tri[tri_len++] = next_val;
    }
}

/* Find largest m such that T_m <= n.  Assume tri is up to date. */
static size_t locate_max_k(idx_t n)
{
    size_t lo = 0, hi = tri_len - 1, best = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (tri[mid] <= (seq_t)n) {
            best = mid;
            lo = mid + 1;
        } else if (mid == 0) {
            break;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

/* Return index of triangular number equal to val within [0, max_idx],
   or -1 if not found.                                          */
static int triangular_index(seq_t val, size_t max_idx)
{
    long long lo = 0, hi = (long long)max_idx;
    while (lo <= hi) {
        long long mid = (lo + hi) >> 1;
        if (tri[mid] == val)
            return (int)mid;
        else if (tri[mid] < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/*  Required external function                                           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* Undefined, but harmless */

    /* Ensure we have all triangular numbers up to n. */
    ensure_tri_up_to(n);

    /* Highest possible triangular component. */
    size_t k_max = locate_max_k(n);

    /* Search k from k_max downwards. */
    for (size_t k = k_max; ; ) {
        seq_t T_k = tri[k];
        idx_t rem = n - (idx_t)T_k;          /* i + j remaining */

        /* Try all i <= k. */
        for (size_t i = 0; i <= k; ++i) {
            seq_t T_i = tri[i];
            if (T_i > rem)
                break;
            seq_t t_j = rem - (idx_t)T_i;    /* check if t_j is triangular */
            int j_idx = triangular_index(t_j, k);
            if (j_idx >= 0)          /* j exists and j <= k */
                return T_k;
        }

        /* Next smaller k */
        if (k == 0)
            break;   /* should never happen for valid inputs */
        --k;
    }

    /* Should never reach here; return 0 as a safe default. */
    return 0;
}
