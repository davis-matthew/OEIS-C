/*
 * A021226  Decimal expansion of 1/222.
 * https://oeis.org/A021226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n)=0 for n=0,1; a(2+3k)=4, a(3+3k)=5, a(4+3k)=0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the decimal expansion of 1/222 has leading zeros. */
    if (n < 2)
        return (seq_t)0;

    /* Compute the position in the repeating 3‑digit cycle. */
    idx_t r = (n - 2) % 3;

    if (r == 0)
        return (seq_t)4;    /* 4 */
    else if (r == 1)
        return (seq_t)5;    /* 5 */
    else /* r == 2 */
        return (seq_t)0;    /* 0 */
}
