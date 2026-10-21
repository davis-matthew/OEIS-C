/*
 * A229593  Number of boomerang patterns appearing in n X n coins, rotation
 *          not allowed.
 * https://oeis.org/A229593
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*floor(n/3)
 * Range:     u8 n <= 29, u16 n <= 443, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. For n < 2 the value is
       undefined in OEIS, but we return 0 for safety. */
    if (n < 2) return 0;

    /* Compute floor(n/3) using integer division. */
    idx_t n_div3 = n / 3;

    /* Convert to seq_t before the multiplication to ensure the
       result is in the correct unsigned precision. */
    seq_t term = ((seq_t)(n - 1)) * (seq_t)n_div3;
    return term;
}
