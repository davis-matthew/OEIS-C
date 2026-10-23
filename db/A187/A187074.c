/*
 * A187074  a(n) = 0 if and only if n is of the form 3*k or 4*k + 2, otherwise
 *          a(n) = 1.
 * https://oeis.org/A187074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 0 if n%3==0 or n%4==2 else 1
 * Range:     u8 n <= 9733, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n mod 3 and n mod 4 */
    uint64_t r3 = n % 3;
    uint64_t r4 = n % 4;

    if (r3 == 0 || r4 == 2)
        return (seq_t)0;   /* term is 0 */
    else
        return (seq_t)1;   /* term is 1 */
}
