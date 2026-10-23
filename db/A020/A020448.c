/*
 * A020448  Expansion of 1/((1-5x)(1-8x)(1-12x)).
 * https://oeis.org/A020448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020448: Expansion of 1/((1-5x)(1-8x)(1-12x))
   Formula (closed form):
       a(n) = 25*5^n/21 - 16*8^n/3 + 36*12^n/7
   Recurrence:
       a(0) = 1
       a(1) = 25
       a(2) = 429
       a(n) = 25*a(n-1) - 196*a(n-2) + 480*a(n-3)   (n >= 3)
*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)429;

    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)25;  /* a(1)   */
    seq_t a2 = (seq_t)429; /* a(2)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t an = 25 * a2 - 196 * a1 + 480 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
