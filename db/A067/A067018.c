/*
 * A067018  Start with a(0)=1, a(1)=4, a(2)=3, a(3)=2; for n>=3, a(n+1) =
 *          mex_i (nim-sum a(i)+a(n-i)), where mex means smallest nonnegative
 *          missing number.
 * https://oeis.org/A067018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A067018
 *
 * Recurrence:
 *   a(0) = 1, a(1) = 4, a(2) = 3, a(3) = 2
 *   for n >= 3:
 *     a(n+1) = mex { a(i) XOR a(n-i) | 0 <= i <= n }
 *   (mex = smallest non‑negative integer not in the set)
 *
 * The recurrence yields a simple closed form for all n >= 4:
 *   a(n) = 0 if n is even
 *   a(n) = 2 if n is odd
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;
    return (seq_t)((n & 1) ? 2 : 0);
}
