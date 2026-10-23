/*
 * A126927  Coordination sequence for 24-dimensional cyclotomic lattice
 *          Z[zeta_39].
 * https://oeis.org/A126927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{24} c_k * binom(n - k + 23, 23) * where c =
 *            [1,15,120,667,2865,10068,29998,77670,177966,363919, *
 *            655692,1001649,1214590,1001649,655692,363919, *
 *            177966,77670,29998,10068,2865,667,120,15,1]
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t c[25] = {
        1,15,120,667,2865,10068,29998,77670,177966,363919,
        655692,1001649,1214590,1001649,655692,363919,
        177966,77670,29998,10068,2865,667,120,15,1
    };
    seq_t sum = 0;
    for (idx_t k = 0; k <= 24 && k <= n; ++k) {
        /* Compute binom(n - k + 23, 23) */
        idx_t N = n - k + 23;          /* N >= 23 */
        seq_t bin = 1;
        for (idx_t i = 1; i <= 23; ++i) {
            bin = bin * (N + 1 - i) / i;
        }
        sum += c[k] * bin;
    }
    return sum;
}
