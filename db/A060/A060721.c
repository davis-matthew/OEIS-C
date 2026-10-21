/*
 * A060721  Singular n X n matrices over GF(7).
 * https://oeis.org/A060721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^(n^2) - ∏_{k=0}^{n-1} (7^n - 7^k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of singular n×n matrices over GF(7). */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
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
    /* the offset is 1; for n==0 return 0 (does not occur in tests) */
    if (n == 0)
        return 0;

    /* 7^n */
    seq_t pow7n = pow_seq_t(7, n);

    /* product of (7^n - 7^k) for k=0..n-1 */
    seq_t prod = 1;
    seq_t pow7k = 1;          /* 7^0 */
    for (idx_t k = 0; k < n; ++k)
    {
        prod *= (pow7n - pow7k);
        pow7k *= 7;           /* advance to 7^(k+1) */
    }

    /* 7^(n^2) */
    idx_t nsq = n * n;
    seq_t total = pow_seq_t(7, nsq);

    return total - prod;
}
