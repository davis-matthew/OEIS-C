/*
 * A194732  Units' digits of the nonzero octagonal numbers.
 * https://oeis.org/A194732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n*(3*n-2) mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Units' digit of the nth nonzero octagonal number */

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    /* The result depends only on n modulo 10.  Perform all arithmetic
       in seq_t so the function works for any seq_t size. */
    idx_t r = n % 10;                /* r ∈ [0,9] */
    seq_t rr = (seq_t)r;             /* convert to seq_t (small number) */
    seq_t val = (rr * ((seq_t)3 * rr - (seq_t)2)) % (seq_t)10;
    return val;
}
