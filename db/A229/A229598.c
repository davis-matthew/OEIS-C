/*
 * A229598  Voids left when packing boomerangs into n X n coins.
 * https://oeis.org/A229598
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-2 - floor(n/3))*(n-2 - floor(n/3)+2) -
 *            floor(n/3)^2 (for n >= 2, offset = 2)
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division by 3 gives the number of multiples of 3 up to n */
    idx_t m = n / 3;                     /* m = floor(n/3) */
    /* number of indices with positive increments */
    idx_t P = (n - 2) - m;               /* P = (n-2) - floor(n/3) */

    seq_t pos_sum = (seq_t)P * (seq_t)(P + 2);   /* sum of positive increments = P*(P+2) */
    seq_t neg_sum = (seq_t)m * (seq_t)m;         /* sum of negative increments = m^2 */

    return (seq_t)1 + pos_sum - neg_sum;
}
