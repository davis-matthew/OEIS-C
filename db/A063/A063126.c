/*
 * A063126  Dimension of the space of weight 2n cusp forms for Gamma_0( 58 ).
 * https://oeis.org/A063126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063126 – dimension of weight 2n cusp forms for Γ₀(58).
   For n > 1:  a(n) = (-19 + (-1)^n + 30·n) / 2
   Special case: a(1) = 6
   (The formula yields integer values for all n ≥ 1.) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n = 0 – return 0 */
    if (n == 1)
        return (seq_t)6;

    /* Compute the numerator using signed 64‑bit arithmetic */
    long long signed_n = (long long)n;
    long long parity = ((n & 1ULL) ? -1LL : 1LL);  /* (-1)^n */
    long long numerator = -19LL + parity + 30LL * signed_n;

    /* numerator is always even for n ≥ 2; divide by 2 */
    long long result = numerator / 2LL;
    return (seq_t)result;
}
