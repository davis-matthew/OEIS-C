/*
 * A191543  Dispersion of (floor(8*n/3)), by antidiagonals.
 * https://oeis.org/A191543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t get_t(idx_t i)
{
    /* returns the i-th element of the complement sequence t */
    idx_t k_s = 1;                         /* index into s */
    seq_t s_val = ((seq_t)8 * k_s) / 3;    /* s(k_s) */
    idx_t missing = 0;                     /* how many missing numbers seen */

    for (idx_t m = 1;; ++m) {
        if (m == s_val) {                  /* m belongs to s */
            ++k_s;
            s_val = ((seq_t)8 * k_s) / 3;  /* next s value */
        } else {                           /* m is missing */
            ++missing;
            if (missing == i)
                return (seq_t)m;
        }
    }
}
/* A191543: Dispersion of floor(8*n/3) by antidiagonals.
   The array D is defined by D[row i , col j] =
   (t[i], s(t[i]), s(s(t[i])), ...), where
   s(k) = floor(8*k/3) and t is the complement of s.
   The nth term a(n) is the nth element of D read by
   antidiagonals starting from the northwest corner. */

seq_t a(idx_t n)
{
    /* locate the coordinates (i,j) of the nth antidiagonal element */
    idx_t k = 1;
    idx_t cum = 1; /* cumulative count of elements up to diagonal k */
    while (cum < n) {
        ++k;
        cum += k;
    }
    idx_t prev_cum = cum - k; /* elements before diagonal k */
    idx_t p = n - prev_cum;   /* 1‑based index within diagonal k */
    idx_t i = p;              /* row index */
    idx_t j = k - p + 1;      /* column index */

    /* first element in row i is t(i) */
    seq_t val = get_t(i);

    /* apply s j-1 times to get the jth element in that row */
    for (idx_t r = 1; r < j; ++r)
        val = ((seq_t)8 * val) / 3;

    return val;
}
