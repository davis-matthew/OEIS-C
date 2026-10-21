/*
 * A212760  Number of (w,x,y,z) with all terms in {0,...,n}, w even, and x = y
 *          + z.
 * https://oeis.org/A212760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(2*n+3+(-1)^n)/8
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by separating the even/odd cases to avoid large intermediate values. */
seq_t a(idx_t n)
{
    /* Convert index to seq_t once. */
    seq_t termA = (seq_t)n + 1;       /* n + 1 */
    seq_t termB = (seq_t)n + 2;       /* n + 2 */
    seq_t prod  = termA * termB;      /* (n+1)*(n+2) */

    /* Multiply by the third factor depending on parity of n. */
    if ((n & 1U) == 0)                /* n even: multiply by n+2 */
    {
        prod = prod * termB;          /* (n+1)*(n+2)^2 */
    }
    else                              /* n odd: multiply by n+1 */
    {
        prod = prod * termA;          /* (n+1)^2*(n+2) */
    }

    /* Final division by 4 gives the closed‑form value. */
    return prod / 4U;
}
