/*
 * A131384  Product of the digital sums of n for all the bases 2 to n (a
 *          'digital-sum factorial').
 * https://oeis.org/A131384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A131384
 *  Formula: a(n) = ∏_{p=2..n} ( n - (p-1)*∑_{k≥1}⌊n/p^k⌋ )
 *  (digital‐sum factorial, runs over all bases 2,…,n)
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n=1 (product over empty set) */
    if (n < 2)          /* works for n==0 or n==1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* For each base p from 2 to n compute the digital sum ds_p(n) */
    for (idx_t base = 2; base <= n; ++base) {
        /* sum floors of n/p^k */
        seq_t sum = (seq_t)0;
        idx_t power = base;
        while (power <= n) {
            sum += (seq_t)(n / power);
            /* prevent power overflow */
            if (power > n / base)
                break;
            power *= base;
        }
        /* ds_p(n) = n - (p-1)*sum */
        seq_t ds = (seq_t)n - (seq_t)(base - 1) * sum;
        result *= ds;
    }
    return result;
}
