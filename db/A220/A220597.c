/*
 * A220597  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly two horizontal, vertical and
 *          antidiagonal neighbors, without 3-loops.
 * https://oeis.org/A220597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 7*a(n-2) + 25*a(n-3) - 3*a(n-4) + 9*a(n-5) +
 *            22*a(n-6) - 5*a(n-7) - 2*a(n-8) for n ≥ 9 Initial values:
 *            a(1)=1, a(2)=7, a(3)=65, a(4)=471, a(5)=3469, a(6)=25747,
 *            a(7)=190425, a(8)=1408823
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial eight terms directly */
    if (n <= 8) {
        static const seq_t init[8] = {
            1, 7, 65, 471, 3469, 25747, 190425, 1408823
        };
        return init[n - 1];
    }

    /* Circular buffer holding the last 8 computed terms.
       prev[0] = a(k-8), ..., prev[7] = a(k-1) when computing a(k).   */
    seq_t prev[8];
    prev[0] = 1;            /* a(1) */
    prev[1] = 7;            /* a(2) */
    prev[2] = 65;           /* a(3) */
    prev[3] = 471;          /* a(4) */
    prev[4] = 3469;         /* a(5) */
    prev[5] = 25747;        /* a(6) */
    prev[6] = 190425;       /* a(7) */
    prev[7] = 1408823;      /* a(8) */

    for (idx_t k = 9; k <= n; ++k) {
        seq_t next = 6 * prev[7] + 7 * prev[6] + 25 * prev[5]
                 - 3 * prev[4] + 9 * prev[3] + 22 * prev[2]
                 - 5 * prev[1] - 2 * prev[0];

        /* Shift the buffer left and insert the new term at the end */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = prev[7];
        prev[7] = next;
    }

    return prev[7]; /* a(n) */
}
