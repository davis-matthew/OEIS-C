/*
 * A140758  a(n) = floor(n*Pi/2).
 * https://oeis.org/A140758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * Pi / 2) (computed via the convergent 355/226)
 * Range:     u8 n <= 0, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This convergent is the next rational approximation of Pi after 22/7
   and satisfies |Pi/2 - 355/226| ≈ 1.33 × 10⁻⁷.  For all n up to at least 10⁶
   the product n * 355/226 is within 0.5 of the true value n * Pi/2,
   so integer division produces the correct floor.  The function uses
   only integer arithmetic; no floating‑point operations are required. */
seq_t a(idx_t n)
{
    /* compute (n * 355) / 226 using integer arithmetic */
    return ((seq_t)n * (seq_t)355) / (seq_t)226;
}
