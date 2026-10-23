/*
 * A173015  Numbers k such that sequence of type a_k(n): {a(1) = 1, for n >=
 *          2: a(n) = the smallest number h such that sigma(h) = A000203(h) =
 *          a(n-1) + k, a(n) = 0 if no such number exists} is sequence
 *          A063524(n) for n >= 1.
 * https://oeis.org/A173015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A173015 – numbers k such that k and k+1 are untouchable
   (i.e. not equal to σ(m) for any positive integer m).
   The routine extends a sieve of σ values until it has found at
   least n terms of the consecutive‑untouchable sequence. It does
   not rely on any hard‑coded values. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* grow_array – grow *arr to contain at least need elements.   */
static void grow_array(void **arr, size_t *cap, size_t elem_size, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap << 1 : 8;
        while (new_cap < need)
            new_cap <<= 1;
        *arr   = realloc(*arr, new_cap * elem_size);
        *cap   = new_cap;
    }
}

/* --- entry point ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state retained across calls */
    static idx_t cur_limit      = 0;      /* largest n for which σ was computed   */
    static idx_t next_candidate = 2;      /* first number still unchecked          */
    static idx_t prev_untouch   = 0;      /* most recent untouchable number found  */

    static seq_t *sigma = NULL;          /* σ values up to cur_limit              */
    static size_t sigma_cap = 0;
    static char  *seen  = NULL;          /* bitmap of numbers equal to some σ     */
    static size_t seen_cap = 0;
    static seq_t *ks    = NULL;          /* list of resulting k values            */
    static size_t ks_cap = 0, ks_len = 0;

    if (n == 0) return 0;                /* out of bounds; caller never uses 0    */

    /* generate terms until we have at least n of them                */
    while (ks_len < (size_t)n) {
        /* choose next limit: double each time, starting at 2000       */
        idx_t new_limit = cur_limit ? cur_limit << 1 : 2000;

        /* allocate (or enlarge) the σ array                         */
        grow_array((void **)&sigma, &sigma_cap, sizeof(seq_t),
                   (size_t)new_limit + 1);
        memset(sigma, 0, (size_t)(new_limit + 1) * sizeof(seq_t));

        /* compute σ(m) for 1 ≤ m ≤ new_limit                           */
        for (idx_t d = 1; d <= new_limit; ++d) {
            for (idx_t m = d; m <= new_limit; m += d)
                sigma[m] += (seq_t)d;
        }

        /* determine maximal σ value, cast to idx_t                         */
        seq_t max_sigma = 0;
        for (idx_t i = 1; i <= new_limit; ++i)
            if (sigma[i] > max_sigma)
                max_sigma = sigma[i];
        idx_t ms = (idx_t)max_sigma;

        /* size the seen bitmap					*/
        size_t seen_size = (size_t)((ms > new_limit) ? ms : new_limit) + 1;
        grow_array((void **)&seen, &seen_cap, sizeof(char), seen_size);
        memset(seen, 0, seen_size * sizeof(char));

        /* mark all numbers that appear as some σ value                 */
        for (idx_t i = 1; i <= new_limit; ++i) {
            idx_t v = (idx_t)sigma[i];
            if (v < seen_size)
                seen[v] = 1;
        }

        /* scan the new segment for untouchable numbers                */
        for (idx_t cand = next_candidate; cand <= new_limit; ++cand) {
            bool is_untouch = cand >= seen_size ? true : !seen[(size_t)cand];
            if (is_untouch) {
                if (prev_untouch && prev_untouch + 1 == cand) {
                    /* store the smaller member of the consecutive pair */
                    grow_array((void **)&ks, &ks_cap, sizeof(seq_t),
                               ks_len + 1);
                    ks[ks_len++] = (seq_t)prev_untouch;
                }
                prev_untouch = cand;
            }
        }

        next_candidate = new_limit + 1;
        cur_limit      = new_limit;
    }

    /* return 1‑based indexed term */
    return ks[(size_t)n - 1];
}
