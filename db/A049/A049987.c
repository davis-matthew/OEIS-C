/*
 * A049987  a(n) is the number of arithmetic progressions of 4 or more
 *          positive integers, strictly increasing with sum <= n.
 * https://oeis.org/A049987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of strictly increasing arithmetic progressions of
 *            length ≥4 whose sum of terms is ≤ n. This is computed by
 *            iterating over the progression length L (≥4), the common
 *            difference d (≥1) and the first term a (≥1), counting all
 *            triples (L,d,a) such that S = L*a + d * C ≤ n with C = L*(L-1)/2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external function required by the OEIS harness */
seq_t a(idx_t n)
{
    seq_t count = 0;                      /* result accumulator */

    /* iterate over progression lengths */
    for (idx_t L = 4; L <= n; ++L) {
        /* minimal sum for given L : 1 + 2 + … + L = L*(L+1)/2   */
        idx_t minSum = (L * (L + 1)) / 2;
        if (minSum > n)                     /* no longer progressions fit */
            break;

        /* constant part of the sum: C = L*(L-1)/2 */
        idx_t C = (L * (L - 1)) / 2;
        if (C == 0)                          /* should never happen for L≥2 */
            continue;

        /* maximum allowed difference d such that at least a=1 fits */
        idx_t dmax = (n >= L) ? (n - L) / C : 0;
        if (dmax == 0)                        /* nothing for this L */
            continue;

        /* for each difference count admissible first terms a */
        for (idx_t d = 1; d <= dmax; ++d) {
            /* remaining sum after forgetting the d-part */
            idx_t rest = n - d * C;
            /* maximal first term a with this d */
            idx_t a_max = rest / L;           /* a_max ≥ 1 because rest ≥ L */
            count += (seq_t)a_max;           /* count all a = 1 … a_max */
        }
    }

    return count;
}
