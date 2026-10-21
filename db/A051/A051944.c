/*
 * A051944  a(n) = C(n)*(4*n+1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A051944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) * (4*n + 1) Catalan(n) = 1 for n = 0
 *            Catalan(n) = Catalan(n-1) * 2*(2*n - 1) / (n + 1) for n >= 1
 *            a(0) = 1, a(1) = 5, a(2) = 18, … (OEIS A051944)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t catalan = 1;                 /* C(0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Multiply by 2*(2*i-1) then divide by (i+1) exactly */
        catalan = catalan * (seq_t)(2 * (2 * i - 1));
        catalan = catalan / (seq_t)(i + 1);
    }
    /* a(n) = C(n) * (4*n + 1) */
    seq_t factor = (seq_t)4 * n + 1;   /* compute 4*n + 1 in seq_t to avoid overflow */
    return catalan * factor;
}
