/*
 * A171367  Antidiagonal sums of triangle of Stirling numbers of 2nd kind
 *          A048993.
 * https://oeis.org/A171367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0..n} S(k, n-k),  where S is Stirling numbers of the
 * second kind.  The function builds the Stirling triangle row by row, adding
 * each entry to the appropriate antidiagonal index, and memoises all
 * computed values.  This uses O(n) memory (two rows) and O(n²) time.
 * Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=2, ...
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <new>
#endif

/* Helper to allocate an array of seq_t.  Works for C and C++ (handles
 * arbitrary‑precision types that need a constructor). */
static seq_t *alloc_seq_array(size_t n)
{
    if (n == 0) return NULL;
#if defined(__cplusplus)
    return new seq_t[n]();          /* value‑initialise (zero for builtins) */
#else
    return (seq_t *)malloc(sizeof(seq_t) * n);
#endif
}

/* Helper to free an array allocated with alloc_seq_array. */
static void free_seq_array(seq_t *p)
{
    if (!p) return;
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* Stored antidiagonal sums.  a_cache[n] holds a(n). */
static seq_t *a_cache      = NULL;
static size_t a_cache_size = 0;      /* allocated size of a_cache */

/* Previous Stirling row (S(m‑1, k)). */
static seq_t *prev_row = NULL;
static size_t prev_len = 0;          /* length of prev_row == last_m + 1 */

/* Last fully processed row index (m). */
static idx_t last_m = 0;

/* Grow a_cache so it can hold indices up to 2*needed (worst case). */
static void ensure_cache_size(idx_t needed)
{
    idx_t required = needed * 2 + 1;
    if (a_cache_size >= (size_t)required) return;

    size_t new_size = (size_t)required;
    seq_t *new_cache = alloc_seq_array(new_size);

    /* copy existing data */
    for (size_t i = 0; i < a_cache_size; ++i)
        new_cache[i] = a_cache[i];

    /* initialise new slots to zero */
    for (size_t i = a_cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    free_seq_array(a_cache);
    a_cache = new_cache;
    a_cache_size = new_size;
}

/* Compute all a(n) up to and including n. */
static void compute_up_to(idx_t n)
{
    /* one‑time initialisation */
    if (a_cache == NULL) {
        a_cache = alloc_seq_array(1);    /* holds a(0) */
        a_cache[0] = (seq_t)1;           /* a(0) = 1 */

        prev_row = alloc_seq_array(1);    /* S(0,0) = 1 */
        prev_row[0] = (seq_t)1;
        prev_len = 1;
        last_m   = 0;
        a_cache_size = 1;
    }

    if (n <= last_m) return;   /* already computed */

    ensure_cache_size(n);

    for (idx_t m = last_m + 1; m <= n; ++m) {
        size_t curr_len = (size_t)(m + 1);
        seq_t *curr = alloc_seq_array(curr_len);

        /* S(m,0) = 0 for m>0 */
        curr[0] = (seq_t)0;

        /* Compute S(m,k) for k=1..m */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = (seq_t)0;   /* k · S(m‑1,k) */
            if ((idx_t)k < prev_len) {
                term1 = prev_row[(size_t)k] * (seq_t)k;
            }
            seq_t term2 = (seq_t)0;   /* S(m‑1,k‑1) */
            if ((idx_t)(k - 1) < prev_len) {
                term2 = prev_row[(size_t)(k - 1)];
            }
            curr[(size_t)k] = term1 + term2;
        }

        /* Add this row to the antidiagonal sums */
        for (idx_t k = 0; k <= m; ++k) {
            idx_t idx = m + k;          /* antidiagonal index */
            a_cache[(size_t)idx] += curr[(size_t)k];
        }

        /* Prepare for next iteration */
        free_seq_array(prev_row);
        prev_row = curr;
        prev_len = curr_len;
    }

    last_m = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[(size_t)n];
}
