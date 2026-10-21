/*
 * A077905  Expansion of 1/(1 - x^2 - x^3 + x^4).
 * https://oeis.org/A077905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A077905:  Expansion of 1 / (1 - x² - x³ + x⁴)
   Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=1
                a(n) = a(n‑2) + a(n‑3) – a(n‑4)   for n ≥ 4
*/

seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2,3 */
    const seq_t base[4] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1 };

    if (n < 4)
        return base[(int)n];

    /* Use a circular buffer of the last four values */
    seq_t a0 = base[0];   /* a(n-4)  */
    seq_t a1 = base[1];   /* a(n-3)  */
    seq_t a2 = base[2];   /* a(n-2)  */
    seq_t a3 = base[3];   /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a(i-2) + a(i-3) - a(i-4) */
        seq_t next = a2 + a1 - a0;

        /* slide the window: discard oldest, shift others, insert new one */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* a(n) is now in a3 */
}
