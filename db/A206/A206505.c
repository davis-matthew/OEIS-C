/*
 * A206505  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in 2 X 2 subblocks nondecreasing rightwards and
 *          downwards.
 * https://oeis.org/A206505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 81 * a(2) = 549 * a(3) = 3399 * a(4) = 19797 * a(5) =
 *            111495 * a(6) = 614367 * a(7) = 3337695 * a(n) = 9·a(n‑1) –
 *            18·a(n‑2) – 15·a(n‑3) + 36·a(n‑4) * – 20·a(n‑5) + 11·a(n‑6) –
 *            2·a(n‑7) (n ≥ 8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[8] = {
        0,                  /* dummy for 0‑index */
        81, 549, 3399,
        19797, 111495,
        614367, 3337695
    };

    if (n < 8)
        return init[n];

    /* a1 … a7 correspond to a(n‑7) … a(n‑1)   (initially a(1)…a(7) ) */
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];
    seq_t next = 0;

    /* iterate from n = 8 up to the requested n */
    for (idx_t i = 8; i <= n; ++i) {
        /* recurrence */
        next = 9 * a7 - 18 * a6 - 15 * a5 + 36 * a4
               - 20 * a3 + 11 * a2 - 2 * a1;
        /* shift the window for the next step */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = next;
    }
    return next;
}
