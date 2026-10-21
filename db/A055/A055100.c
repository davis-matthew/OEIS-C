/*
 * A055100  In the game of backgammon, the number of blot hitting rolls when
 *          the blot is n points away, n=1,2,...,24.
 * https://oeis.org/A055100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for OEIS A055100: number of dice rolls that hit a blot n points away
 * in backgammon (1 ≤ n ≤ 24).  The count is over the 36 ordered dice outcomes
 * (i,j) with 1 ≤ i,j ≤ 6; a double (i==j) yields possible distances i,2i,3i,4i
 * while a non‑double yields i, j, and i+j.  */
seq_t a(idx_t n)
{
    /* distance outside the backgammon board cannot be hit */
    if (n < 1 || n > 24) return 0;

    seq_t cnt = 0;
    for (idx_t i = 1; i <= 6; ++i) {
        for (idx_t j = 1; j <= 6; ++j) {
            if (i == j) {
                seq_t v = (seq_t)i;
                if (n == v || n == v*2 || n == v*3 || n == v*4) ++cnt;
            } else {
                if (n == i || n == j || n == i + j) ++cnt;
            }
        }
    }
    return cnt;
}
