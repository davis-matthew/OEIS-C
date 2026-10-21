/*
 * A071900  1/4 times the number of n X n 0..7 matrices with MM' mod 8 = I,
 *          where M' is the transpose of M and I is the n X n identity matrix.
 * https://oeis.org/A071900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n^2 - 1) * n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the conjecture a(n) = 2^{n(n-1)/2} * A071303(n) and
   the observation that A071303(n) = ∏_{k=2}^{n} (k * 2^k) = n! * 2^{n(n+1)/2 - 1}.
   Consequently a(n) = 2^{n^2 - 1} * n! . */

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Exponent of 2: n^2 - 1 */
    /* n is uint64_t so the product fits in 64‑bit for n <= 10^4 */
    uint64_t exp = (uint64_t)n * (uint64_t)n;  /* n^2 */
    if (exp == 0) return fact;                 /* n == 0; unused but safe */
    --exp;                                     /* n^2 - 1 */

    /* Multiply by 2^exp.  For built‑in integer types the left shift
       works directly; for arbitrary‑precision types the shift operator
       is typically defined as a left bit shift. */
    seq_t result = fact;
    result <<= exp;   /* shift left by 'exp' bits, effectively multiplying by 2^exp */

    return result;
}
