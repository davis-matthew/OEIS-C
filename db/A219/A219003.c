/*
 * A219003  Unmatched value maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 2 X n array.
 * https://oeis.org/A219003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 9*a(n-3) (n>3) a(1)=1, a(2)=10,
 *            a(3)=46 Recurrence derived from OEIS A219003
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t a_start[4] = {0, 1, 10, 46};

seq_t a(idx_t n)
{
    if (n <= 3) {
        /* Directly return the pre‑computed initial terms. */
        return a_start[n];
    }

    /* Use the recurrence to compute successive terms. */
    seq_t prev3 = a_start[1];   /* a(1)   */
    seq_t prev2 = a_start[2];   /* a(2)   */
    seq_t prev1 = a_start[3];   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 4 * prev1;   /* 4*a(i-1) */
        next += 9 * prev3;        /* +9*a(i-3) */
        next -= 3 * prev2;        /* -3*a(i-2) */

        /* Shift the window for the next iteration. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;   /* a(n) */
}
