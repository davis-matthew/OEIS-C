/*
 * A171005  a(n) = (n+1)*(n-1)!/2.
 * https://oeis.org/A171005
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n-1)!/2 for n >= 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any idx_t n >= 3.
   For n < 3 the result is undefined (any value may be returned). */
seq_t a(idx_t n)
{
    /* A small optimisation: if n == 3 we can compute directly,
       but the general loop works fine for all n >= 3. */
    seq_t fact = 1;

    /* Compute (n-1)! */
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Multiply by (n+1) and divide by 2.  The division is exact. */
    fact *= (seq_t)(n + 1);
    fact /= (seq_t)2;

    return fact;
}
