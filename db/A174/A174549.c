/*
 * A174549  a(n) = (2*n-1)! + (2*n)!.
 * https://oeis.org/A174549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)! + (2*n)! = (1+2*n)*(2*n-1)! (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* no definition for n=0 */

    /* exponent of the factorial that we really need */
    idx_t target = 2 * n - 1;                  /* (2*n-1)!  */

    /* cache the largest factorial already computed */
    static idx_t max_fact   = 0;
    static seq_t fact       = (seq_t)1;        /* factorial(0) = 1 */

    if (target > max_fact) {
        /* grow the cache upward */
        for (idx_t i = max_fact + 1; i <= target; ++i)
            fact = fact * ((seq_t)i);
        max_fact = target;
    } else if (target < max_fact) {
        /* shrink the cache – recompute from 1 */
        fact = (seq_t)1;
        for (idx_t i = 1; i <= target; ++i)
            fact = fact * ((seq_t)i);
        max_fact = target;
    }

    /* a(n) = (1+2*n)*(2*n-1)!  */
    seq_t result = fact * ((seq_t)(1 + 2 * n));
    return result;
}
