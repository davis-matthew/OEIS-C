/*
 * A077148  Smallest k such that there are n numbers m relatively prime to n
 *          in range n < m < k.
 * https://oeis.org/A077148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077148:  a(n) = smallest k such that there are n integers
    m with n < m < k and gcd(n,m)=1.
    The function searches for the n‑th coprime of n greater than n
    and returns one more than that integer. */

/* Euclidean algorithm for seq_t values */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; undefined for 0, return 0 */
        return (seq_t)0;

    seq_t N = (seq_t)n;    /* n as the arithmetic type */
    seq_t m = N + (seq_t)1;/* first candidate > n */

    idx_t count = 0;

    for (;;)
    {
        if (gcd_seq(N, m) == (seq_t)1)        /* m coprime to n ? */
        {
            ++count;
            if (count == n)                  /* found the n-th coprime */
                break;
        }
        m = m + (seq_t)1;                     /* next integer */
    }

    return m + (seq_t)1;        /* k = m + 1, as required */
}
