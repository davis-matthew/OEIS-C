/*
 * A101612  n! * Sum[k=floor(n/2)..n, 1/k].
 * https://oeis.org/A101612
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=floor(n/2)}^n (1/k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the sequence exactly for any n ≥ 2. */

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is undefined; return 0. */
    if (n < 2)
        return 0;

    /* Compute n! as a seq_t. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Sum the integer terms n! / k from k = floor(n/2) up to k = n. */
    idx_t start = n / 2;          /* floor(n/2) */
    seq_t sum = 0;
    for (idx_t k = start; k <= n; ++k)
        sum += fact / (seq_t)k;

    return sum;
}
