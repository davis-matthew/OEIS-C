/*
 * A050446  Table read by ascending antidiagonals: T(n, m) giving total degree
 *          of n-th-order elementary symmetric polynomials in m variables.
 * https://oeis.org/A050446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) is the n-th term of the table T(n,m) read by ascending
 *            antidiagonals with reversed order inside each diagonal, where
 *            T(n,m) is defined by T(0,m) = 1 for all m, T(n,0) = 1 for all n,
 *            T(n,m) = T(n,m-1) + Σ_{k=0..⌊(n-1)/2⌋} T(2k,m-1) * T(n-1-2k,m)
 * Range:     u8 n <= 46, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t **T = NULL;          /* 2‑D table T[n][m] */
static idx_t last_size = 0;       /* current dimension of T */

/* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
static inline idx_t diag_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n) lo = mid;
        else hi = mid;
    }
    return lo;
}

/* Rebuild table for dimensions up to 'size' (size x size) */
static void build_table(idx_t size)
{
    seq_t **tab = (seq_t **)malloc(size * sizeof(seq_t *));
    for (idx_t i = 0; i < size; ++i)
        tab[i] = (seq_t *)calloc(size, sizeof(seq_t)); /* init to 0 */

    /* base cases */
    for (idx_t n = 0; n < size; ++n) tab[n][0] = 1;          /* T(n,0)=1 */
    for (idx_t m = 0; m < size; ++m) tab[0][m] = 1;          /* T(0,m)=1 */

    /* recurrence */
    for (idx_t m = 1; m < size; ++m) {
        for (idx_t n = 1; n < size; ++n) {
            seq_t val = tab[n][m - 1];
            idx_t maxk = (n - 1) / 2;
            for (idx_t k = 0; k <= maxk; ++k)
                val += tab[2 * k][m - 1] * tab[n - 1 - 2 * k][m];
            tab[n][m] = val;
        }
    }

    /* free old table */
    if (T != NULL) {
        for (idx_t i = 0; i < last_size; ++i) free(T[i]);
        free(T);
    }
    T = tab;
    last_size = size;
}

/* public function */
seq_t a(idx_t n)
{
    idx_t d = diag_of(n);                     /* diagonal index   */
    idx_t pos = n - d * (d + 1) / 2;          /* offset in diag   */
    idx_t i = d - pos;                        /* n-index in T     */
    idx_t j = pos;                            /* m-index in T     */

    if (last_size <= d)                       /* need larger table */
        build_table(d + 1);

    return T[i][j];
}
