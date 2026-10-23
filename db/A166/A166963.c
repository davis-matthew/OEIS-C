/*
 * A166963  Number of permutations in S_{2n} avoiding 123 and 1432 whose
 *          matrices are 180-degree symmetric.
 * https://oeis.org/A166963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166963:
   a(n) = 2*a(n-1) + a(n-3) + a(n-4) - a(n-5)
   a(0)=1, a(1)=2, a(2)=6, a(3)=12, a(4)=27, a(5)=61
   (O(n) iterative computation, no dynamic allocation)
*/

seq_t a(idx_t n)
{
    /* Base values for 0 ≤ n ≤ 5 */
    static const seq_t base[6] = {1, 2, 6, 12, 27, 61};

    if (n < 6)
        return base[n];

    /* last[0] = a(k), last[1] = a(k-1), … , last[5] = a(k-5)  */
    seq_t last[6];
    last[0] = base[5]; /* a(5) */
    last[1] = base[4]; /* a(4) */
    last[2] = base[3]; /* a(3) */
    last[3] = base[2]; /* a(2) */
    last[4] = base[1]; /* a(1) */
    last[5] = base[0]; /* a(0) */

    for (idx_t k = 6; k <= n; ++k) {
        seq_t val = 2 * last[0] + last[2] + last[3] - last[4];
        /* shift right: a(k) becomes a(k-1), … */
        for (int i = 5; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = val; /* a(k) */
    }

    return last[0];
}
