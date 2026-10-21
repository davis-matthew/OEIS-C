/*
 * A087098  Partial sums of A087100.
 * https://oeis.org/A087098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) - 5*a(n-4) + 5*a(n-5) +
 *            2*a(n-6) - 2*a(n-7) (for n > 6) Initial values: a(0)=0, a(1)=1,
 *            a(2)=12, a(3)=15, a(4)=37, a(5)=44, a(6)=77
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t init[7] = {0, 1, 12, 15, 37, 44, 77};

    if (n <= 6) {
        return init[n];
    }

    /* Rolling buffer for the last seven terms */
    seq_t v[7];
    v[0] = init[0]; v[1] = init[1]; v[2] = init[2];
    v[3] = init[3]; v[4] = init[4]; v[5] = init[5]; v[6] = init[6];

    for (idx_t k = 7; k <= n; ++k) {
        seq_t next = v[6] + 4*v[5] - 4*v[4] - 5*v[3] + 5*v[2] + 2*v[1] - 2*v[0];

        /* Shift the window: remove v[0], and add next as v[6] */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = next;
    }

    return v[6];
}
