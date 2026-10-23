/*
 * A126275  Moment of inertia of all magic squares of order n.
 * https://oeis.org/A126275
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * (n^4 - 1) / 12
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 61, u64 n <= 2459,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 2; the offset of the OEIS entry is 2. */
    if (n < 2) return 0;

    /* Compute the expression using a 128‑bit intermediate type
       to avoid overflow before the final division.  This works for
       any seq_t that can represent the result (including 64‑bit
       and larger). */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;            /* n^2            */
    unsigned __int128 n4   = n2 * n2;            /* n^4            */
    unsigned __int128 num  = n2 * (n4 - 1);      /* n^2 * (n^4-1)  */
    unsigned __int128 res  = num / 12;           /* divide by 12   */
    return (seq_t)res;
}
