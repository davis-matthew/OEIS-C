/*
 * A187852  Number of 4-step king-knight's tours (piece capable of both kinds
 *          of moves) on an n X n board summed over all starting positions.
 * https://oeis.org/A187852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3504*n^2 - 17748*n + 21996 for n >= 6 Base values for n
 *            <= 5: a(1) = 0 a(2) = 24 a(3) = 1400 a(4) = 7620 a(5) = 20952
 *            (Values for n > 5 are given by the quadratic formula.)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for n = 1..5 */
    static const seq_t base[5] = {0, 24, 1400, 7620, 20952};

    if (n <= 5) {
        /* n is unsigned, so n-1 is valid for n>=1 */
        return base[n - 1];
    }

    /* For n >= 6 use the quadratic formula */
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)3504 * nn * nn - (seq_t)17748 * nn + (seq_t)21996;
    return result;
}
