/*
 * A071358  Least k>0 such that the last digit of (n+k)^n is the same as the
 *          last digit of n^n.
 * https://oeis.org/A071358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071358: Least positive k such that the last digit of (n+k)^n
   equals the last digit of n^n.
   The sequence is periodic with period 20:
   (10, 6, 10, 2, 10, 8, 10, 4, 10, 10, 10, 2, 10, 2, 10, 2, 10, 4, 10, 10). */

seq_t a(idx_t n)
{
    static const seq_t period[20] = {
        10, 6, 10, 2, 10, 8, 10, 4, 10, 10,
        10, 2, 10, 2, 10, 2, 10, 4, 10, 10
    };
    /* n is 1‑indexed; the period repeats every 20 indices. */
    return period[(n - 1) % 20];
}
