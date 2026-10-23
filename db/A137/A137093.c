/*
 * A137093  Numbers k such that k and k^2 use only the digits 2, 4, 5 and 6.
 * https://oeis.org/A137093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = numbers k such that k and k² consist only of the digits
 *            2,4,5,6. The sequence is almost the simple family a(L) = (2·10^L
 *            – 5)/3 (L ≥ 1) which yields 5, 65, 665, 6665, … . The first few
 *            terms are sporadic: a(1)=2 a(3)=25 a(10)=25625465 a(11)=65226242
 *            All other terms are given by the above f...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* value for n < 1 is irrelevant */

    /* handle the sporadic small terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)65;
    if (n == 5) return (seq_t)665;
    if (n == 6) return (seq_t)6665;
    if (n == 7) return (seq_t)66665;
    if (n == 8) return (seq_t)666665;
    if (n == 9) return (seq_t)6666665;
    if (n == 10) return (seq_t)25625465;
    if (n == 11) return (seq_t)65226242;

    /* n ≥ 12 : use the closed‑form pattern */
    /* pattern index L satisfies: a(n) = (2·10^L – 5)/3,  L = n – 4        */
    idx_t L = n - 4;           /* L ≥ 8 for the first valid case (a(12))      */
    seq_t value = (seq_t)5;    /* a(L=1) = 5                                   */

    /* iterative recurrence: a(L+1) = 10·a(L) + 15                            */
    for (idx_t i = 2; i <= L; ++i)
    {
        value = value * (seq_t)10 + (seq_t)15;
    }

    return value;
}
