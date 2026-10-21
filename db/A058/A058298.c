/*
 * A058298  Triangle n!/(n-k), 1 <= k < n, read by rows.
 * https://oeis.org/A058298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 78, u64 n <= 209,
 *            u128 n <= 566, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058298 – Triangle n!/(n-k), 1 <= k < n, read by rows.
   The function locates the row r and column c such that
   the term with index n (offset 2) is T(r,c) = r!/(r-c).
   The value is computed as the product of 1..r with the factor (r-c)
   omitted, avoiding overflow as long as the final result fits
   into seq_t.  This holds for all n>=2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* below offset – undefined */
    /* zero‑based position in the flattened triangle */
    idx_t p    = n - 2;
    idx_t pos  = p + 1;           /* 1‑based position */
    /* find minimal row r with (r-1)*r/2 >= pos */
    idx_t r = 2;
    while ((r - 1) * r / 2 < pos) ++r;
    /* column index (1‑based) */
    idx_t c = pos - ((r - 2) * (r - 1) / 2);
    /* denominator = r - c */
    idx_t d = r - c;
    /* compute r!/(r-c) by multiplying all i=1..r except i=d */
    seq_t val = 1;
    for (idx_t i = 1; i <= r; ++i) {
        if (i == d) continue;
        val *= (seq_t)i;
    }
    return val;
}
