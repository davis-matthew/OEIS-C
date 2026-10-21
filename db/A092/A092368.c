/*
 * A092368  a(0)=0, a(1)=1, a(2)=1. For n>2, let R_{k}(n) = the digit reversal
 *          of n in base k (R_{k}(n) is written in base 10). a(n) is the
 *          smallest value of R_{k}(n) arising if k=2,..,n-1.
 * https://oeis.org/A092368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   computes a(n) = min_{2≤k<n} rev_k(n), where rev_k(n) is the
 *            digit reverse of n written in base k (interpreted as a base‑10
 *            integer)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* explicit base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* use the largest possible integer type for intermediate results;
       idx_t is guaranteed to be at least 64‑bit. */
    idx_t best = (idx_t)-1;  /* maximum value for unsigned type */

    /* examine each base k from 2 to n-1 */
    for (idx_t k = 2; k < n; ++k)
    {
        idx_t rev = 0;          /* reversed number in base 10 */
        idx_t tmp = n;          /* copy of n to extract digits */

        /* build rev by repeatedly taking remainders */
        while (tmp > 0)
        {
            idx_t digit = tmp % k;
            rev = rev * k + digit;
            tmp = tmp / k;
        }

        if (rev < best) best = rev;
    }

    return (seq_t)best;
}
