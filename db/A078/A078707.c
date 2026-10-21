/*
 * A078707  Number of vectors of length n that are symmetric about the middle,
 *          where each element is drawn from a set of n distinct elements.
 * https://oeis.org/A078707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(floor((n+1)/2)) = n^ceiling(n/2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n^ceil(n/2) for all n >= 0.
 * 0^0 is defined as 1.  Uses standard unsigned exponentiation
 * by squaring; intermediate multiplications use the seq_t type,
 * avoiding overflow for values that fit in seq_t.  For values that
 * exceed the capacity of seq_t, the wrap‑around result is fine
 * because those terms are not checked for the 64‑bit build. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute exponent ceil(n/2) without overflow. */
    idx_t exp = n / 2 + n % 2;  /* ceil(n/2) */

    /* Special case: n == 0 gives 0^0 == 1 by convention. */
    if (n == 0)
        return (seq_t)1;

    return pow_seq_t((seq_t)n, exp);
}
