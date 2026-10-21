/*
 * A086398  a(1)=1; a(n)=a(n-1)+2 if n is in the sequence; a(n)=a(n-1)+2 if n
 *          and (n-1) are not in the sequence; a(n)=a(n-1)+4 if n is not in
 *          the sequence but (n-1) is in the sequence.
 * https://oeis.org/A086398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n) = a(n-1)+2  if n is already a term or if (n-1) is not a term;
   a(n) = a(n-1)+4  if n is not a term but (n-1) is a term.
   Sequence is generated incrementally and memoised; works for any
   seq_t (unsigned integer or arbitrary‐precision class) and any
   idx_t == uint64_t.  This implementation is valid C11 and C++17.  */
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of helper used during generation. */
static bool _seq_contains_value(idx_t val);

/* Static storage for already computed terms:
   a_vals[0] holds a(1), a_vals[1] holds a(2), …   */
static seq_t *a_vals   = NULL;   /* pointer to array of seq_t */
static size_t a_cap   = 0;       /* allocated slots in a_vals   */
static size_t a_len   = 0;       /* number of terms stored      */

/* Binary search for a seq_t value inside the already computed terms. */
static bool _seq_contains_value(idx_t v)
{
    seq_t target = (seq_t)v;     /* convert index to seq_t value */
    size_t lo = 0, hi = a_len - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t mid_val = a_vals[mid];
        if (mid_val == target)
            return true;
        if (mid_val < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

/* Main function computing a(n).  Memoises all previous terms. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 safely */
        return (seq_t)0;

    /* Initialise storage on first use. */
    if (a_cap == 0) {
        a_cap = 1; /* space for a(1) */
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_vals)
            exit(EXIT_FAILURE);
        a_vals[0] = (seq_t)1;   /* a(1) = 1 */
        a_len = 1;
    }

    /* Grow the array if necessary to hold up to index n. */
    if ((size_t)n > a_cap) {
        size_t new_cap = (size_t)n; /* exactly enough for n terms */
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        a_vals = tmp;
        a_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    while (a_len < (size_t)n) {
        idx_t cur_idx      = (idx_t)(a_len + 1); /* 1‑based index of new term */
        idx_t prev_idx     = cur_idx - 1;       /* index of previous term */
        seq_t prev_val     = a_vals[a_len - 1];  /* a(cur_idx-1) */

        bool cur_in  = _seq_contains_value(cur_idx);
        bool prev_in = _seq_contains_value(prev_idx);

        seq_t inc = (seq_t)2;
        if (!cur_in && prev_in)
            inc = (seq_t)4;

        a_vals[a_len] = prev_val + inc;
        a_len++;
    }

    return a_vals[n - 1];
}
