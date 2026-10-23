/*
 * A123006  Expansion of x^2/(1 -2*x -121*x^2).
 * https://oeis.org/A123006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123006: Expansion of x^2/(1 - 2*x - 121*x^2)
   a(1) = 0
   a(2) = 1
   a(n) = 2*a(n-1) + 121*a(n-2)  for n >= 3 */
 
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) */
    if (n == 2) return (seq_t)1;   /* a(2) */

    seq_t a1 = (seq_t)0;           /* a(1) */
    seq_t a2 = (seq_t)1;           /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = (seq_t)2 * a2 + (seq_t)121 * a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
