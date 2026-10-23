/*
 * A021466  Decimal expansion of 1/462.
 * https://oeis.org/A021466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4), a(0)=0, a(1)=0, a(2)=2, a(3)=1,
 *            a(4)=6
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2u)
        return (seq_t)0;

    static const seq_t cycle[6] = { (seq_t)2, (seq_t)1, (seq_t)6, (seq_t)4, (seq_t)5, (seq_t)0 };
    /* The sequence repeats the pattern 2,1,6,4,5,0 after the leading zeros. */
    idx_t i = (n - 2u) % 6u;
    return cycle[i];
}
