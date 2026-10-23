/*
 * A152538  Triangle read by rows, A027293 * (A152537 * 0^(n-k)).
 * https://oeis.org/A152538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = p(r-j) * d(j) where r = floor((sqrt(8n+1)-1)/2), j = n -
 *            r(r+1)/2 p(k) = partition number, d(k) = A152537(k)
 * Range:     u8 n <= 64, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- cached tables ---------------------------------------------------- */
static seq_t *p_cache = NULL;      /* partition numbers p[0..p_len-1]  */
static size_t p_len = 0;           /* current size of p_cache          */

static seq_t *d_cache = NULL;      /* A152537 values d[0..d_len-1]     */
static size_t d_len = 0;           /* current size of d_cache          */

static seq_t *pow2_cache = NULL;   /* powers of two 2^i                */
static size_t pow2_len = 0;        /* current size of pow2_cache       */

static seq_t *a_cache = NULL;      /* flattened triangle values        */
static size_t a_cap  = 0;          /* currently allocated capacity     */
static idx_t rows_precalculated = 0; /* number of rows already computed */

/* --- helper: filled cache for partition numbers up to 'max' --------- */
static void ensure_partitions(idx_t max)
{
    if (p_len > max) return;
    /* enlarge p_cache */
    seq_t *tmp = (seq_t *)realloc(p_cache, (max + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    p_cache = tmp;
    p_len  = max + 1;
    /* initialize new elements */
    for (idx_t i = 0; i <= max; ++i) p_cache[i] = (seq_t)0;
    p_cache[0] = (seq_t)1;
    /* classic DP: for each part size, add to higher sums */
    for (idx_t part = 1; part <= max; ++part) {
        for (idx_t j = part; j <= max; ++j) {
            p_cache[j] += p_cache[j - part];
        }
    }
}

/* --- helper: powers of two up to 'max' -------------------------------- */
static void ensure_pow2(idx_t max)
{
    if (pow2_len > max) return;
    seq_t *tmp = (seq_t *)realloc(pow2_cache, (max + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    pow2_cache = tmp;
    if (pow2_len == 0) {
        pow2_cache[0] = (seq_t)1;
        pow2_len = 1;
    }
    for (idx_t i = pow2_len; i <= max; ++i) {
        pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
    }
    pow2_len = max + 1;
}

/* --- helper: A152537 (d) up to 'max' --------------------------------- */
static void ensure_d(idx_t max)
{
    if (d_len > max) return;
    seq_t *tmp = (seq_t *)realloc(d_cache, (max + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    d_cache = tmp;
    if (d_len == 0) {
        d_cache[0] = (seq_t)1;
        d_len = 1;
    }
    /* ensure dependencies */
    ensure_partitions(max);
    ensure_pow2(max);

    for (idx_t n = d_len; n <= max; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < n; ++k) {
            sum += p_cache[n - k] * d_cache[k];
        }
        d_cache[n] = pow2_cache[n] - sum;
    }
    d_len = max + 1;
}

/* --- helper: find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 -------- */
static idx_t row_of(idx_t n)
{
    /* binary search on r */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n) {
        high <<= 1;               /* double until value >= n */
    }
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* --- ensure all rows up to 'needed_row' are in a_cache ---------------- */
static void ensure_rows(idx_t needed_row)
{
    while (rows_precalculated <= needed_row) {
        idx_t r = rows_precalculated; /* next row index to compute */

        ensure_partitions(r);
        ensure_d(r);

        /* new total size after adding this row */
        size_t new_len = (size_t)((r + 1) * (r + 2) / 2);
        if (a_cap < new_len) {
            seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            a_cache = tmp;
            a_cap = new_len;
        }

        idx_t start = r * (r + 1) / 2;
        for (idx_t j = 0; j <= r; ++j) {
            a_cache[start + j] = p_cache[r - j] * d_cache[j];
        }
        rows_precalculated++;
    }
}

/* --- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    ensure_rows(r);
    return a_cache[n];
}
