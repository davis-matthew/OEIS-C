/*
 * A192988  Smallest number whose representation requires n pentagonal numbers
 *          with greedy algorithm.
 * https://oeis.org/A192988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192988 (OEIS)
   a(1) = 1
   a(n+1) = ceil(a(n)/3) * (3*ceil(a(n)/3) - 1) / 2 + a(n)   (n ≥ 1)
   Offset: 1
*/
seq_t a(idx_t n)
{
    /* For n = 1 the value is fixed. */
    if (n <= 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;                  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* k = ceil(val / 3)  ==  (val + 2) / 3 for unsigned integers */
        seq_t k = (val + (seq_t)2) / (seq_t)3;

        /* inc = k * (3*k - 1) / 2  */
        seq_t inc = k * ((seq_t)3 * k - (seq_t)1);
        inc /= (seq_t)2;

        val += inc;
    }
    return val;
}
