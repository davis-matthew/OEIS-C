/*
 * A126132  a(n) = number of k's, 1<=k<=n, where d(k) is equal to any divisor
 *          of n, where d(k) is the number of positive divisors of k.
 * https://oeis.org/A126132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of k, 1 <= k <= n, such that d(k) divides n, where
 *            d(k) is the number of positive divisors of k
 * Range:     u8 n <= 287, u16 n <= 9334, u32 n <= 9334, u64 n <= 9334,
 *            u128 n <= 9334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square–root function using only integer arithmetic. */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* Highest power of two <= sqrt(max_uint64) */
    idx_t bit = ((idx_t)1ULL << 31);  /* 2^31 * 2 = 2^32 < 2^64, fit in uint64_t */
    while (bit > 0) {
        idx_t next = r + bit;
        /* Use division to avoid overflow: next*next <= n  <=>  next <= n/next */
        if (next <= n / next) {
            r = next;
        }
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t root = int_sqrt_u64(k);
        idx_t dcnt = 0;
        for (idx_t i = 1; i <= root; ++i) {
            if (k % i == 0) {
                idx_t q = k / i;
                if (i == q)
                    dcnt += 1;
                else
                    dcnt += 2;
            }
        }
        if (dcnt != 0 && n % dcnt == 0)
            ++count;
    }
    return count;
}
