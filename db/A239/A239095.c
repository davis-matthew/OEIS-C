/*
 * A239095  a(n) = (n^7 - 21*n^3 + 20*n)/210.
 * https://oeis.org/A239095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 - 21*n^3 + 20*n)/210
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative combinatorial form: a(n) = 8*(C(n+4,7)+C(n+3,7)+C(n+2,7)) */

/* Computes a(n) for OEIS A239095.  Uses a combinatorial representation
 * that avoids overflow for small seq_t types by computing binomial
 * coefficients with 128-bit intermediate arithmetic.  The result is
 * cast to seq_t before returning. */
seq_t
a(idx_t n)
{
    /* For n < 3 the formula yields 0.  The following loop works for all n. */
    unsigned __int128 sum = 0;         /* Accumulator for the sum of binomials */

    for (int offset = 2; offset <= 4; ++offset) {
        uint64_t m = n + offset;       /* Compute n + offset as uint64_t */
        if (m < 7)       /* C(m,7) is 0 when m < 7 */
            continue;

        /* Compute binomial C(m,7) using multiplicative formula with
         * 128‑bit intermediate to avoid overflow. */
        unsigned __int128 term = 1;
        for (uint64_t i = 1; i <= 7; ++i) {
            term = term * (m - 7 + i); /* Multiply by the next factor in the numerator */
            term = term / i;           /* Exact division by the denominator component */
        }
        sum += term;
    }

    sum *= 8;     /* Multiply the sum of binomials by 8 as per the closed form */
    return (seq_t)sum;  /* Truncate (or convert) to seq_t */
}
