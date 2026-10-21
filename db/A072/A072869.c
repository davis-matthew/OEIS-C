/*
 * A072869  a(n) = sigma(sigma(n)-n), where sigma = A000203, sum of the
 *          divisors of n.
 * https://oeis.org/A072869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(sigma(n) - n) (OEIS A072869)
 * Range:     u8 n <= 59, u16 n <= 7559, u32 n <= 9269, u64 n <= 9269,
 *            u128 n <= 9269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    sigma(k) = sum of the positive divisors of k (A000203) */
/*    n >= 2 (offset 2)                                */

static seq_t sigma_of(idx_t n)
{
    seq_t sum = 0;
    idx_t d = 1;
    /* iterate over divisors up to sqrt(n); avoid overflow by comparing
       d <= n / d instead of d*d <= n                                 */
    while (d <= n / d) {
        if ((n % d) == 0) {
            sum += (seq_t)d;
            idx_t other = n / d;
            if (other != d) sum += (seq_t)other;
        }
        d++;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant */

    seq_t sigma_n = sigma_of(n);          /* sigma(n)   */
    seq_t diff = sigma_n - (seq_t)n;      /* sigma(n)-n */
    idx_t m = (idx_t)diff;                /* index for second sigma */

    return sigma_of(m);                   /* sigma(m)   */
}
