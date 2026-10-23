/*
 * A171166  A polyspiral path: a(n) represents the n-th vertex of a lattice
 *          path with an infinite number of finite square spirals.
 * https://oeis.org/A171166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial‐sum definition of A171166.  
   For k = 1,2,3,... let L = 3k−1 and S = 2k−1.  
   The auxiliary sequence b(k) consists of:  
        L, L, S, S, S−1, S−1, …, 1, 1   (each integer appears twice).  
   Then a(n) = Σ_{i=1..n} b(i),  with a(0) = 0. */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t total = 0;
    idx_t produced = 0;
    idx_t cycle = 1;

    while (produced < n) {
        idx_t L = 3 * cycle - 1;        /* first value in this cycle */
        idx_t S = 2 * cycle - 1;        /* last value before 1 */

        /* produce L twice */
        for (int rep = 0; rep < 2 && produced < n; ++rep) {
            total += (seq_t)L;
            ++produced;
        }

        /* produce S .. 1, each twice */
        for (idx_t val = S; val > 0 && produced < n; --val) {
            for (int rep = 0; rep < 2 && produced < n; ++rep) {
                total += (seq_t)val;
                ++produced;
            }
        }

        ++cycle;
    }

    return total;
}
