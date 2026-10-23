/*
 * A116971  a(n) = (35*2^((2*(3*n+2) + 2)/3) - 2*(3*n+2) - 46)/9.
 * https://oeis.org/A116971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=10, a(1)=56, a(2)=242
   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3)   for n >= 3
*/

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)10;
    if (n == (idx_t)1) return (seq_t)56;
    if (n == (idx_t)2) return (seq_t)242;

    seq_t a0 = (seq_t)10;   // a(n-3)
    seq_t a1 = (seq_t)56;   // a(n-2)
    seq_t a2 = (seq_t)242;  // a(n-1)

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t next = (seq_t)6 * a2 - (seq_t)9 * a1 + (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
