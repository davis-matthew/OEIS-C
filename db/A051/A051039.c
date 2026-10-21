/*
 * A051039  4-Stohr sequence.
 * https://oeis.org/A051039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n-59 for n > 4; Initial values: a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=8
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;
    if (n == 4) return 8;

    /* For n > 4 the closed‑form works */
    return (seq_t)n * 15 - 59;
}
