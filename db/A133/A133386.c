/*
 * A133386  Number of forests of labeled rooted trees with n nodes, containing
 *          exactly 2 trees of height one, all others having height zero.
 * https://oeis.org/A133386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) * Stirling2(n-1,3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stirling numbers of the second kind are given by
   S(k,3) = (3^k - 3*2^k + 3)/6, for k >= 0. */

static seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Small n produce zero because S(k,3)=0 for k<3. */
    if (n < 4)
        return 0;

    idx_t k = n - 1;                   /* exponent for 3^k and 2^k          */
    seq_t pow3 = pow_seq_small((seq_t)3, k);
    seq_t pow2 = pow_seq_small((seq_t)2, k);

    seq_t numerator = pow3 - (seq_t)3 * pow2 + (seq_t)3;
    seq_t S = numerator / (seq_t)6;    /* exact division */

    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn - (seq_t)1) * S;
    return res;
}
