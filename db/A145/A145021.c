/*
 * A145021  a(n) = number of different positive integers that can be formed
 *          from different groupings of expressions of the form n op1 n op2 n
 *          op3 n, where each of op1, op2 and op3 are addition, subtraction,
 *          multiplication or division.
 * https://oeis.org/A145021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of distinct positive integers obtainable from
   expressions of the form n op1 n op2 n op3 n, where each opi is one of
   +, –, *, /.  The parenthesization may be done arbitrarily.
   For n ≥ 7:
     a(n) = 30 if n is odd, 31 if n is even.
   The first six values are:
     a(1)=4, a(2)=10, a(3)=20, a(4)=25, a(5)=27, a(6)=29. */
seq_t a(idx_t n)
{
    /* handle the first six exceptional cases */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)25;
    if (n == 5) return (seq_t)27;
    if (n == 6) return (seq_t)29;

    /* for n ≥ 7: parity decides between 30 and 31 */
    return (seq_t)((n & (idx_t)1) ? 30 : 31);
}
