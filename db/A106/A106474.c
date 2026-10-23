/*
 * A106474  a(n) = A006579(4n+4)/4.
 * https://oeis.org/A106474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A006579(4n+4)/4 = Sum_{k=0..4n+2} gcd(4n-k+3, k+1)/4
 * Range:     u8 n <= 13, u16 n <= 2517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    /* Euclidean algorithm, works for any unsigned seq_t type */
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* upper limit of k is 4n+2 (fits in idx_t for the intended ranges) */
    idx_t limit = 4 * n + 2;

    for (idx_t k = 0; k <= limit; ++k)
    {
        seq_t a1 = (seq_t)(4 * n - k + 3);  /* 4n - k + 3 */
        seq_t a2 = (seq_t)(k + 1);          /* k + 1 */
        seq_t g  = gcd_seq(a1, a2);
        sum += g;
    }

    /* the sum is always divisible by 4 */
    return sum / 4;
}
