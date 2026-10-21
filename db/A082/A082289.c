/*
 * A082289  Expansion of x^4*(2+x)/((1+x)*(1-x)^5).
 * https://oeis.org/A082289
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/96)*(2*(n-2)*n*(3*n^2 - 10*n + 4) + 3*(-1)^n - 3)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 444, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the exact value for any n >= 4. */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* value below the offset is irrelevant */

    /* cast index to seq_t for safe arithmetic */
    seq_t n_seq   = (seq_t)n;
    seq_t n_minus = (seq_t)(n - 2);

    /* compute inner polynomial: 3*n^2 - 10*n + 4 */
    seq_t n_sq   = n_seq * n_seq;          /* n^2 */
    seq_t inner  = 3 * n_sq;               /* 3*n^2 */
    inner -= 10 * n_seq;                   /* -10*n */
    inner += 4;                            /* +4 */

    /* compute product: 2*(n-2)*n*inner */
    seq_t prod   = n_minus * n_seq;        /* (n-2)*n */
    prod       *= inner;                  /* *inner */
    prod       *= 2;                      /* *2 */

    /* apply parity adjustment: subtract 6 if n is odd */
    if ((n & 1) != 0)
        prod -= 6;

    /* finally divide by 96 */
    return prod / 96;
}
