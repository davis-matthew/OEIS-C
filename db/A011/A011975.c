/*
 * A011975  Covering numbers C(n,3,2).
 * https://oeis.org/A011975
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (n/3) * ceil((n-1)/2) )
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The inner ceil((n-1)/2) is simply n/2 with integer division.
   Thus a(n) = ceil( n * (n/2) / 3 ).
   Ceil(x/3) is (x + 2) / 3 when x is an integer. */
seq_t a(idx_t n)
{
    /* ceil((n-1)/2) */
    seq_t b = (seq_t)(n / 2);

    /* product n * b */
    seq_t prod = (seq_t)n * b;

    /* ceil(prod / 3) = (prod + 2) / 3 */
    return (prod + 2) / 3;
}
