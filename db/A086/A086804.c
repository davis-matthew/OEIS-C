/*
 * A086804  a(0)=0; for n > 0, a(n) = (n+1)^(n-2)*2^(n^2).
 * https://oeis.org/A086804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^(n-2) * 2^(n^2), a(0)=0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary exponentiation for seq_t values.  Works for any
   arithmetic type that supports +, -, *, /, %, <<, >>. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The first term as defined in the OEIS entry. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* For n == 1 the formula gives (2)^(‑1), so we return 1 explicitly. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* (n+1)^(n-2) */
    seq_t part1 = pow_seq_t((seq_t)(n + (idx_t)1),
                           (idx_t)(n - (idx_t)2));

    /* 2^(n^2)   (n^2 fits in idx_t for the required range). */
    idx_t exp2 = n * n;              /* n^2 as unsigned 64‑bit */
    seq_t part2 = pow_seq_t((seq_t)2, exp2);

    return part1 * part2;
}
