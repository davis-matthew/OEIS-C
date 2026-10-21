/*
 * A094293  At the n-th step, append the number n and n copies of the list of
 *          all preceding terms, starting with an empty list.
 * https://oeis.org/A094293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094293.
   Recursive definition:
       a(1) = 1
       For k ≥ 2, let L_{k-1} be the list of the first
       len_{k-1} terms.  The (k‑th) step appends the number k
       and then k copies of L_{k-1}.  Thus
           len_k = (k+1) * len_{k-1} + 1   (len_0 = 0).
       The n‑th term a(n) is found by locating the step s such that
       len_{s-1} < n ≤ len_s.  If n == len_{s-1}+1 then a(n)=s;
       otherwise a(n) = a( ((n-(len_{s-1}+1)-1) mod len_{s-1}) + 1 ). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static array holding cumulative lengths len_k.
   len[0] = 0, len[1] = 1, etc.  The array grows automatically
   as larger n are requested. */
static idx_t *len_arr = NULL;
static size_t len_cnt = 0;   /* number of stored len values (>=1) */
static size_t len_cap = 0;   /* current allocation size */

/* Ensure that len_arr contains all len_k such that len_k ≥ target.
   The len values are computed using 128‑bit arithmetic to avoid
   overflow for the range we need. */
static void ensure_len(idx_t target)
{
    if (len_arr == NULL) {
        len_cap = 16;
        len_arr = (idx_t *)malloc(len_cap * sizeof(idx_t));
        if (!len_arr) return;     /* allocation failure – will crash later */
        len_arr[0] = 0;           /* len_0 */
        len_cnt   = 1;            /* we have len[0] and we will compute len[1] next */
    }

    /* Compute new len values until we reach target */
    while (len_arr[len_cnt - 1] < target) {
        if (len_cnt == len_cap) {
            len_cap *= 2;
            len_arr = (idx_t *)realloc(len_arr, len_cap * sizeof(idx_t));
            if (!len_arr) return;
        }
        idx_t k = len_cnt;                    /* current step k (len_k will be stored) */
        idx_t prev = len_arr[k - 1];
        __int128 p = (__int128)(k + 1) * prev + 1;
        len_arr[k] = (idx_t)p;                /* overflow simply wraps in idx_t, not a problem here */
        ++len_cnt;
    }
}

/* Recursive helper to compute a(n).  The argument is 1‑based. */
static seq_t a_recursive(idx_t n)
{
    if (n == 0) return 0;                       /* not expected, but safe guard */
    /* Find step s such that len_{s-1} < n ≤ len_s.  len[0] = 0. */
    size_t s = 1;
    while (len_arr[s] < n) ++s;

    idx_t prev_len = len_arr[s - 1];             /* len_{s-1} */
    if (n == prev_len + 1)                       // the appended number at this step
        return (seq_t)s;

    /* Position inside the copies of the previous list. */
    idx_t offset = n - (prev_len + 1);           // 1 ≤ offset ≤ s * prev_len
    idx_t pos_in_prev = ((offset - 1) % prev_len) + 1;
    return a_recursive(pos_in_prev);
}

/* Public function as required by the interface. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence is defined for n ≥ 1 */
    ensure_len(n);
    return a_recursive(n);
}
