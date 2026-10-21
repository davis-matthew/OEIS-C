/*
 * A097609  Triangle read by rows: T(n,k) is number of Motzkin paths of length
 *          n having k horizontal steps at level 0.
 * https://oeis.org/A097609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 119, u32 n <= 350, u64 n <= 1127,
 *            u128 n <= 3915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A097609: Triangle read by rows
   Formula:  T(0,0)=1,
   T(n,k) = (k>0?T(n-1,k-1):0) + Σ_{j=1}^{n-1-k} T(n-1,k+j)     (0≤k≤n)
   a(n) is the flattening of this triangle: a(r*(r+1)/2 + k) = T(r,k).  */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Forward declarations */
static void ensure_rows(idx_t r);
static void add_row(idx_t n);

/* Global storage for the triangle in row‑major order */
static idx_t max_row = 0;          /* highest row currently stored */
static size_t flat_cap = 0;        /* capacity of flat array */
static size_t flat_size = 0;       /* number of elements stored */
static seq_t *flat = NULL;         /* flattened triangle values */

static size_t offsets_cap = 0;     /* capacity of offsets array */
static idx_t *offsets = NULL;      /* offsets[i] = starting index of row i in flat */

static size_t suffix_cap = 0;      /* capacity of suffix array */
static seq_t *suffix = NULL;       /* temporary storage for suffix sums */

/* Initialise the data structures (row 0) */
static void initialise(void)
{
    if (flat)
        return;                      /* already initialised */

    /* row 0 */
    flat_cap = 64;
    flat = (seq_t*)malloc(flat_cap * sizeof(seq_t));
    flat[0] = 1;
    flat_size = 1;

    offsets_cap = 4;
    offsets = (idx_t*)malloc(offsets_cap * sizeof(idx_t));
    offsets[0] = 0;

    suffix_cap = 4;
    suffix = (seq_t*)malloc(suffix_cap * sizeof(seq_t));
    suffix[0] = 0;

    max_row = 0;
}

/* Ensure that rows 0..r are stored */
static void ensure_rows(idx_t r)
{
    initialise();
    if (max_row >= r)
        return;
    for (idx_t n = max_row + 1; n <= r; ++n)
        add_row(n);
}

/* Add row n (length n+1) to the data structures */
static void add_row(idx_t n)          /* n ≥ 1 */
{
    size_t prev_len = n;               /* length of row n‑1 */
    idx_t prev_off = offsets[n - 1];
    seq_t *prev = flat + prev_off;

    /* Grow flat array if necessary */
    if (flat_size + (n + 1) > flat_cap) {
        flat_cap = flat_cap ? flat_cap << 1 : 128;
        while (flat_cap < flat_size + n + 1)
            flat_cap <<= 1;
        flat = (seq_t*)realloc(flat, flat_cap * sizeof(seq_t));
    }

    /* Grow offsets array if necessary */
    if (max_row + 1 >= offsets_cap) {
        offsets_cap = offsets_cap ? offsets_cap << 1 : 8;
        while (offsets_cap <= max_row + 1)
            offsets_cap <<= 1;
        offsets = (idx_t*)realloc(offsets, offsets_cap * sizeof(idx_t));
    }

    /* Grow suffix array if necessary */
    if (suffix_cap < prev_len + 1) {
        suffix_cap = suffix_cap ? suffix_cap << 1 : 8;
        while (suffix_cap < prev_len + 1)
            suffix_cap <<= 1;
        suffix = (seq_t*)realloc(suffix, suffix_cap * sizeof(seq_t));
    }

    /* Compute suffix sums of previous row */
    suffix[prev_len] = 0;
    for (size_t i = prev_len; i-- > 0; ) {
        suffix[i] = prev[i];
        suffix[i] += suffix[i + 1];
    }

    /* Append current row */
    idx_t this_off = flat_size;
    offsets[n] = this_off;

    for (size_t k = 0; k <= n; ++k) {
        seq_t left  = (k > 0) ? prev[k - 1] : 0;
        seq_t right = (k + 1 < n) ? suffix[k + 1] : 0;
        flat[this_off + k] = left + right;
    }

    flat_size += n + 1;
    max_row = n;
}

/* Return the sequence term a(n) */
seq_t a(idx_t n)
{
    /* Find row r and column k such that n = r*(r+1)/2 + k */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    ensure_rows(r);
    return flat[offsets[r] + k];
}
