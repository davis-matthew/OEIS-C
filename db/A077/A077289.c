/*
 * A077289  Triangular numbers that are 1/6 of another triangular number.
 * https://oeis.org/A077289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=6, a(3)=105
   For n ≥ 4: a(n) = 98·a(n−2) – a(n−4) + 7
   (All operations use the type seq_t supplied by the build.) */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 105;

    /* Use a sliding window of the four most recent terms:
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1). */
    seq_t a0 = 0;   /* a(0)   */
    seq_t a1 = 1;   /* a(1)   */
    seq_t a2 = 6;   /* a(2)   */
    seq_t a3 = 105; /* a(3)   */
    seq_t an = 0;   /* placeholder for the current term */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 98*a(i-2) – a(i-4) + 7   */
        an = 98 * a2;
        an -= a0;
        an += 7;

        /* Slide the window one step forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
