/*
 * A124526  Triangle, read by rows, where T(n,k) =
 *          A049020([n/2],k)*A049020([(n+1)/2],k).
 * https://oeis.org/A124526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where Let start(r) = Σ_{i=0}^{r-1} (1 + ⌊i/2⌋)
 *            (offset of first element of row r). Find r and k such that
 *            start(r) ≤ n < start(r+1) and k = n - start(r). Then a(n) =
 *            A049020(⌊r/2⌋,k) * A049020(⌈r/2⌉,k)
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049020(n,k) satisfies the recurrence
        A(n,k) = (k+1)*A(n-1,k) + A(n-1,k-1) + (k+1)*A(n-1,k+1),
   with A(0,0)=1 and A(n,k)=0 for k>n.  */

/*  Standard headers required by the build.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Handle row start offsets (linear index of first element of
    each row of the triangle).  */
static idx_t *row_start = NULL;
static size_t row_start_len = 0;
static size_t row_start_cap = 0;

static void init_row_start(void) {
    if (row_start == NULL) {
        row_start_cap = 64;
        row_start = (idx_t *)malloc(row_start_cap * sizeof(idx_t));
        row_start[0] = 0;      /* row 0 starts at index 0 */
        row_start_len = 1;
    }
}

static void ensure_row_start(idx_t n) {
    init_row_start();
    /* Extend until start[n] is available */
    while (row_start_len <= n) {
        idx_t prev_row = (idx_t)(row_start_len - 1);
        idx_t prev_len  = 1 + (prev_row / 2);
        idx_t new_start = row_start[prev_row] + prev_len;
        if (row_start_len == row_start_cap) {
            row_start_cap <<= 1;
            row_start = (idx_t *)realloc(row_start, row_start_cap * sizeof(idx_t));
        }
        row_start[row_start_len++] = new_start;
    }
}

static idx_t find_row(idx_t n) {
    /* Binary search on the (now guaranteed) row_start array. */
    idx_t lo = 0, hi = (idx_t)(row_start_len - 1);
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;      /* upper mid */
        if (row_start[mid] <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */
/*  Table of A049020 rows.  */
struct BRow {
    seq_t *data;          /* data[0..row] */
};

static struct BRow *Bcache = NULL;
static size_t Bcache_len = 0;   /* number of rows cached */
static size_t Bcache_cap = 0;

static void init_Bcache(void) {
    if (Bcache == NULL) {
        Bcache_cap = 64;
        Bcache = (struct BRow *)malloc(Bcache_cap * sizeof(struct BRow));
        Bcache_len = 0;
    }
}

static void ensure_Bsize(idx_t n) {
    init_Bcache();
    while (Bcache_len <= n) {
        idx_t m = (idx_t)Bcache_len;          /* new row index to compute */
        size_t cur_len = (size_t)(m + 1);
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));

        if (m == 0) {                          /* base case */
            cur[0] = (seq_t)1;
        } else {
            seq_t *prev = Bcache[m - 1].data;   /* length m */
            for (size_t k = 0; k < cur_len; ++k) {
                seq_t term = 0;
                if (k <= m - 1) term += (seq_t)(k + 1) * prev[k];
                if (k > 0)      term += prev[k - 1];
                if (k + 1 <= m - 1) term += (seq_t)(k + 1) * prev[k + 1];
                cur[k] = term;
            }
        }

        if (Bcache_len == Bcache_cap) {
            Bcache_cap <<= 1;
            Bcache = (struct BRow *)realloc(Bcache, Bcache_cap * sizeof(struct BRow));
        }
        Bcache[Bcache_len].data = cur;
        ++Bcache_len;
    }
}

/* ------------------------------------------------------------ */
/*  Main sequence function.  */
seq_t a(idx_t n) {
    /* Locate row r and column k in the flattened triangle. */
    ensure_row_start(n);
    idx_t r = find_row(n);
    idx_t k = n - row_start[r];

    /* Indices of the two A049020 rows that are multiplied. */
    idx_t n0 = r / 2;
    idx_t n1 = (r + 1) / 2;
    idx_t max_idx = (n0 > n1) ? n0 : n1;
    ensure_Bsize(max_idx);

    /* Retrieve the two values.  If k exceeds the row length,
       the value is implicitly zero. */
    seq_t v0 = (k <= n0) ? Bcache[(size_t)n0].data[(size_t)k] : 0;
    seq_t v1 = (k <= n1) ? Bcache[(size_t)n1].data[(size_t)k] : 0;

    /* Return the product. */
    return v0 * v1;
}
