/*
 * A224430  The hyper-Wiener index of the dendrimer D_1[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41490 - 420000*2^n + 81225*4^n*n^2 - 467115*4^n*n/2 +
 *            378669*4^n + 231*2^n*n - 1425*2^n*n^2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements linear recurrence derived from the rational generating function to avoid large intermediates. */
seq_t a(idx_t n)
{
    /* First seven terms (indices 0..6) */
    static const seq_t init[7] = {
        (seq_t)159,
        (seq_t)104448,
        (seq_t)2123802,
        (seq_t)22761810,
        (seq_t)183445458,
        (seq_t)1256801106,
        (seq_t)7758199698
    };

    if (n < 7)
        return init[n];

    seq_t cur[7];
    for (int i = 0; i < 7; ++i)
        cur[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t t1 = (seq_t)19   * cur[6];
        seq_t t2 = (seq_t)150  * cur[5];
        seq_t t3 = (seq_t)636  * cur[4];
        seq_t t4 = (seq_t)1560 * cur[3];
        seq_t t5 = (seq_t)2208 * cur[2];
        seq_t t6 = (seq_t)1664 * cur[1];
        seq_t t7 = (seq_t)512  * cur[0];

        seq_t new_val = t1 - t2 + t3 - t4 + t5 - t6 + t7;

        for (int j = 0; j < 6; ++j)
            cur[j] = cur[j + 1];
        cur[6] = new_val;
    }

    return cur[6];
}
