/*
 * A070051  Number of Bottleneck-Monge matrices with 3 rows. In the formula
 *          below, P=3.
 * https://oeis.org/A070051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n‑1) - 50*a(n‑2) + 120*a(n‑3) - 160*a(n‑4) * +
 *            112*a(n‑5) - 32*a(n‑6) * Recurrence holds for n ≥ 7. * Initial
 *            values: a(1)=8, a(2)=33, a(3)=117, a(4)=384, * a(5)=1194,
 *            a(6)=3558
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 48, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 6 terms */
    static const seq_t init[6] = {8, 33, 117, 384, 1194, 3558};

    if (n <= 6)
        return init[n - 1];

    /* Working buffer holding the last six terms:
     * t[0] = a(n‑6), t[5] = a(n‑1) before computation of a(n). */
    seq_t t[6];
    for (idx_t i = 0; i < 6; ++i)
        t[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 11 * t[5]          /* a(n‑1) */
                  - 50 * t[4]          /* a(n‑2) */
                  + 120 * t[3]         /* a(n‑3) */
                  - 160 * t[2]         /* a(n‑4) */
                  + 112 * t[1]         /* a(n‑5) */
                  - 32 * t[0];         /* a(n‑6) */

        /* Shift buffer */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = next;
    }

    return t[5];
}
