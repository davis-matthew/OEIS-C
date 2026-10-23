/*
 * A157753  Duplicate of A020500.
 * https://oeis.org/A157753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=0; for n>1, a(n)=p if n is a power of a prime p, otherwise
 *            a(n)=1, where p is the smallest prime factor of n
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;            /* a(1) = 0 */

    /* find the smallest prime factor p of n */
    idx_t p = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0)          /* n itself is prime */
        p = n;

    /* check if n is a pure power of p */
    idx_t tmp = n;
    while (tmp % p == 0)
        tmp /= p;

    if (tmp == 1)
        return (seq_t)p;   /* n = p^k, return the smallest prime factor p */
    else
        return (seq_t)1;   /* n has another prime factor */
}
