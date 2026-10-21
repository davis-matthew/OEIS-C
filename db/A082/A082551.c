/*
 * A082551  Denote sigma(n)-n by s(n); a(n)=1 if s(n)>n, a(n)=0 if s(n)=n,
 *          a(n)=-1 if s(n)<n.
 * https://oeis.org/A082551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sign(sigma(n) - n) where sigma(n) is the sum of all
 *            divisors of n. * This is equivalent to a(n) = sign(sigma(n) -
 *            2*n)
 * Range:     i8 n <= 232, i16 n <= 9645, i32 n <= 9645, i64 n <= 9645,
 *            i128 n <= 9645, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define SMIN(a,b) ((a)<(b)?(a):(b))

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑bounds guard, though offset is 1 */
    seq_t sigma = 1;               /* include divisor 1 */
    if (n > 1) sigma += (seq_t)n;  /* include the divisor n itself */

    /* iterate over possible divisor pairs (i, n/i) with i >= 2 */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i) sigma += (seq_t)j;
        }
    }

    seq_t diff = sigma - (seq_t)2 * n;
    if (diff > 0) return (seq_t)1;
    if (diff < 0) return (seq_t)-1;
    return (seq_t)0;
}
