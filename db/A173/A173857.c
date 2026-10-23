/*
 * A173857  Expansion of 3/2 in base phi.
 * https://oeis.org/A173857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 for n == 1; otherwise a(n) = 1 if (n-2)%3 == 1, 0 else.
 *            The sequence is periodic with period 3 after the first term
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    /* (n-2) is at least 0 because n >= 2 here */
    idx_t mod = (n - 2) % 3;
    return (mod == 1) ? (seq_t)1 : (seq_t)0;
}
