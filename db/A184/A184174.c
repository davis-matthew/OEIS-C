/*
 * A184174  Triangle read by rows: T(n,k) is the number of partitions of the
 *          set {1,2,...,n} having k adjacent blocks of size 2, i.e., blocks
 *          of the form (i,i+1) (0 <= k <= floor(n/2)).
 * https://oeis.org/A184174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 71, u64 n <= 181,
 *            u128 n <= 483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184174 – triangle T(n,k) read by rows
   Each entry a(n) corresponds to T(r,c) where r is the row,
   c the column index within that row.
   T(r,c) = Σ_{j=c..⌊r/2⌋} (-1)^(c+j) * C(j,c) * C(r-j,j) * Bell(r-2*j)   */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>

/* ---------- binomial coefficient ------------------------------------ */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* ---------- Bell numbers -------------------------------------------- */
static seq_t *bell = NULL;      /* bell[i] = Bell(i) */
static idx_t bell_len = 0;      /* number of computed Bell numbers */

/* Ensure Bell numbers up to index n are available */
static void ensure_bell(idx_t n)
{
    if (bell_len > n) return;       /* already enough */

    /* allocate or grow the array */
    seq_t *tmp = (seq_t *)realloc(bell, sizeof(seq_t) * (n + 1));
    if (!tmp) { exit(1); }          /* memory allocation failure */
    bell = tmp;

    for (idx_t i = bell_len; i <= n; ++i) {
        if (i == 0) {
            bell[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t up = i - 1;
        for (idx_t k = 0; k <= up; ++k) {
            sum += binom(up, k) * bell[k];
        }
        bell[i] = sum;
    }
    bell_len = n + 1;
}

/* ---------- triangle entry ------------------------------------------ */
static seq_t T_value(idx_t r, idx_t c)
{
    idx_t maxj = r / 2;
    seq_t value = (seq_t)0;
    for (idx_t j = c; j <= maxj; ++j) {
        seq_t term = binom(j, c) * binom(r - j, j) * bell[r - 2 * j];
        if (((c + j) & 1) == 0)
            value += term;
        else
            value -= term;
    }
    return value;
}

/* ---------- converting linear index n -> (row, col) ---------------- */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    idx_t cum = 0;                /* cumulative entries up to previous row */
    while (1) {
        idx_t entries = (r / 2) + 1;  /* number of entries in row r */
        if (n < cum + entries) {
            *row = r;
            *col = n - cum;
            return;
        }
        cum += entries;
        ++r;
    }
}

/* ---------- public entry point -------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_row_col(n, &r, &c);

    ensure_bell(r);             /* Bell numbers up to r are needed */
    return T_value(r, c);       /* triangle value */
}
