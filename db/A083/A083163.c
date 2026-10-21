/*
 * A083163  Define a function f(k) by f(k) = (k*a*b*c...)/(d*e*f...), where
 *          a,b,c,...are those nonzero digits of k which do not divide k and
 *          d,e,f,... are those nonzero digits of k which divide k. a(n) =
 *          Floor[f(k)].
 * https://oeis.org/A083163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( n * Π{d∈digits(n), d≠0, d∤n} d / Π{d∈digits(n),
 *            d≠0, d|n} d )
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (out of range for the sequence) */
    if (n == 0)
        return (seq_t)0;

    idx_t orig = n;          /* Preserve the original value for modulo checks */
    idx_t temp = n;          /* Used to extract digits */

    seq_t numer = (seq_t)1;  /* Product of non‑dividing digits (excluding 0) */
    seq_t denom = (seq_t)1;  /* Product of dividing digits (excluding 0) */

    while (temp != 0)
    {
        unsigned int digit = (unsigned int)(temp % 10);
        temp /= 10;

        if (digit == 0)
            continue;               /* Ignore zero digits */

        /* Check if the digit divides the original number */
        if (orig % digit == 0)
            denom *= (seq_t)digit;   /* Digit divides the number */
        else
            numer *= (seq_t)digit;   /* Digit does NOT divide the number */
    }

    /* Compute floor(n * numer / denom).  Integer division truncates toward 0. */
    seq_t result = ((seq_t)orig * numer) / denom;
    return result;
}
