/*
 * A134176  a(n) = (3/8)*(n-1)*(n-2)*(27*n^2-137*n+180).
 * https://oeis.org/A134176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/8)*(n-1)*(n-2)*(27*n^2-137*n+180)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 88, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is formed in seq_t to avoid overflow in any implementation. */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t sn = n;

    /* Compute the factors of the polynomial. */
    seq_t t1 = sn - 1;                         /* n-1  */
    seq_t t2 = sn - 2;                         /* n-2  */
    seq_t t3 = 27 * sn * sn - 137 * sn + 180;   /* 27n²-137n+180 */

    /* Multiply all factors together. */
    seq_t prod = t1 * t2;          /* (n-1)(n-2) */
    prod = prod * t3;              /* times (27n²-137n+180) */
    prod = prod * 3;               /* times 3  */
    prod = prod / 8;               /* divide by 8 */

    return prod;
}
