/*
 * A094301  a(0)=0, a(1)=1, a(2)=2; for n>2, a(n) = a(n-1) - a(n-2) +
 *          a(n-3)^2.
 * https://oeis.org/A094301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 17, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094301:
   a(0) = 0, a(1) = 1, a(2) = 2;
   a(n) = a(n‑1) - a(n‑2) + a(n‑3)²   for n > 2
   (The recurrence may temporarily produce a negative intermediate value,
   but the result is always non‑negative.  The implementation handles
   the subtraction in a signed‑safe way using unsigned arithmetic.)

   This implementation runs in O(n) time and uses only the three
   most recent terms, so it requires O(1) extra memory. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = 0, a1 = 1, a2 = 2;
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t sq = a0 * a0;          /* a(n-3)² */

        if (a2 >= a1)
            ai = sq + a2 - a1;       /* a(n-1) - a(n-2) + sq   */
        else
            ai = sq - (a1 - a2);     /* (a(n-3)² + a(n-1)) - a(n-2) */

        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
