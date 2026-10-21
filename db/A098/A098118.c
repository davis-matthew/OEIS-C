/*
 * A098118  a(n) = n!*[x^n] (log(x+1) * Sum_{j=0..n} C(2*n,j)*x^j).
 * https://oeis.org/A098118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n-1)!/n!)*\sum_{i=1..n}\sum_{j=1..n} 1/(i+j-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It can also be written as
   a(n) = (2*n)!/n! * \sum_{k=1..n} 1/(n+k),
   which avoids fractions because (2*n)!/n! is divisible by every (n+k).   */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t Q   = 1;

    /* Q will hold the product \prod_{i=n+2}^{2n} i,
       i.e. (2*n)!/(n! (n+1)).  This equals a(1) term.  */
    for (idx_t i = n + 2; i <= 2 * n; ++i)
        Q *= (seq_t)i;

    /* k = 1 term */
    sum += Q;

    /* For k = 2 .. n we use the recurrence
       Q_k = Q_{k-1} * (n+k-1) / (n+k).   */
    for (idx_t k = 2; k <= n; ++k) {
        Q *= (seq_t)(n + k - 1);   /* multiply by previous denominator   */
        Q /= (seq_t)(n + k);       /* divide by current denominator      */
        sum += Q;                  /* add the new term                   */
    }

    return sum;
}
