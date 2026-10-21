/*
 * A065186  a(1)=1, a(2)=3, a(3)=5, a(4)=2, a(5)=4; for n > 5, a(n) = a(n-5) +
 *          5.
 * https://oeis.org/A065186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+5, a(1)=1, a(2)=3, a(3)=5, a(4)=2, a(5)=4
 *            Equivalent: a(n) = base[(n-1)%5] + 5*((n-1)/5) with base =
 *            {1,3,5,2,4}
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base_vals[5] = { (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)2, (seq_t)4 };

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for n < 1 the result is undefined */
    idx_t r = (n - 1) % 5;                 /* 0 ≤ r < 5  */
    seq_t block = ((seq_t)(n - 1)) / 5;    /* number of full periods before n */
    return base_vals[r] + (seq_t)5 * block;
}
