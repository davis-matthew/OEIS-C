/*
 * A166972  Triangle T(n,k) read by rows: T(n,k) = (n-k+1)*T(n-1,k-1) +
 *          (3*k-2)*k*T(n-1,k), initialized by T(n,1) = T(n,n) = 1.
 * https://oeis.org/A166972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Triangle T(n,k) with 1 ≤ k ≤ n, defined by T(n,1) = T(n,n) = 1
 *            T(n,k) = (n-k+1) * T(n-1,k-1) + (3*k-2)*k * T(n-1,k) The
 *            sequence a(n) is T(n,k) read in row‑major order, i.e. the exact
 *            term of the triangle having linear index n (a(1)=1, a(2)=1, …)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 109,
 *            u128 n <= 282, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stdlib.h>

/* Big integer / integer types are provided by the build
   (seq_t, idx_t). */

static seq_t **tri_rows = NULL;      /* pointers to each triangle row */
static idx_t  tri_rows_alloc = 0;    /* allocated length of tri_rows */
static idx_t  tri_max_row = 0;       /* highest row computed so far */

/* ---- integer square root for 0 <= x <= UINT64_MAX ---------------------- */

/* Binary search square root using unsigned __int128 for safety. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;   /* sqrt(UINT64_MAX) < 2^32 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)x) return mid;
        if (sq < (unsigned __int128)x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Determine the row r (1‑based) that contains the linear index n. */
static inline idx_t row_of_n(idx_t n)
{
    /* 8*n+1 fits in 64 bits for the values used by the problem. */
    idx_t U = 8 * n + 1;
    idx_t sqrtU = isqrt_u64(U);
    idx_t r = (sqrtU + 1) / 2;
    /* Correct for perfect triangular indices: 8*n+1 a square. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    if (prev >= n) --r;
    return r;
}

/* Allocate a new triangle row of length `len` and store it.  */
static void add_row(idx_t len)
{
    /* Expand rows pointer array if necessary */
    if (tri_rows_alloc <= len) {
        size_t new_alloc = len + 1;   /* +1 so that index == len fits */
        seq_t **tmp = (seq_t**)realloc(tri_rows, new_alloc * sizeof(seq_t*));
        if (!tmp) exit(1);           /* abort on out‑of‑memory */
        tri_rows = tmp;
        tri_rows_alloc = new_alloc;
    }

    /* Allocate and compute the new row */
    seq_t *row = (seq_t*)malloc((size_t)len * sizeof(seq_t));
    if (!row) exit(1);

    row[0] = 1;                    /* T(len,1) = 1 */
    if (len > 1) row[len-1] = 1;    /* T(len,len) = 1 */

    if (len > 2) {
        seq_t *prev = tri_rows[len-1];
        for (idx_t i = 1; i < len-1; ++i) { /* 0‑based index i corresponds to k=i+1 */
            seq_t part1 = (seq_t)(len - i) * prev[i-1];
            seq_t part2 = (seq_t)((3*i + 1) * (i + 1)) * prev[i];
            row[i] = part1 + part2;
        }
    }

    tri_rows[len] = row;
    tri_max_row = len;
}

/* ------------------------------------------------------------ */
/* Public API */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside defined offset (offset 1) */

    /* Find the row and column (1‑based) for index n */
    idx_t r = row_of_n(n);                  /* row number (≥1) */
    unsigned __int128 prev_count = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_count;        /* column number (1‑based) */

    /* Compute missing rows up to r */
    if (r > tri_max_row)
        for (idx_t rr = tri_max_row + 1; rr <= r; ++rr)
            add_row(rr);

    seq_t *row = tri_rows[r];
    return row[k-1];        /* 0‑based array index */
}
