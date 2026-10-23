/*
 * A134171  a(n) = (9/2)*(n-1)*(n-2)*(n-3).
 * https://oeis.org/A134171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9/2)*(n-1)*(n-2)*(n-3)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 986, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Terms for n = 1, 2, 3 are zero by definition. */
    if (n <= 3)
        return 0;

    /* Compute (9/2)*(n-1)*(n-2)*(n-3) avoiding intermediate overflow. */
    seq_t t1 = (seq_t)(n - 1);
    seq_t t2 = (seq_t)(n - 2);
    seq_t t3 = (seq_t)(n - 3);

    seq_t prod = t1 * t2;   /* (n-1)*(n-2) */
    prod *= t3;            /* (n-1)*(n-2)*(n-3) */
    prod /= 6;             /* divide by 6 (factorial of 3) */
    prod *= 27;            /* multiply by 27 = 9*3 */

    return prod;
}
