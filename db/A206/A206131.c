/*
 * A206131  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 47*a(n-2) + 41*a(n-3) - 10*a(n-4) for n > 5
 *            Initial values: a(1) = 564 a(2) = 6668 a(3) = 91924 a(4) =
 *            1269044 a(5) = 17521788
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a simple linear recurrence; computation is O(n) and exact */
    static const seq_t init[5] = {
        564ULL,      /* a(1) */
        6668ULL,     /* a(2) */
        91924ULL,    /* a(3) */
        1269044ULL,  /* a(4) */
        17521788ULL  /* a(5) */
    };

    if (n <= 5) {
        return init[n - 1];
    }

    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];

    for (idx_t i = 6; i <= n; i++) {
        seq_t next = 17 * a5 - 47 * a4 + 41 * a3 - 10 * a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
