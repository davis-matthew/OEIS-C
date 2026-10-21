/*
 * A063926  Number of edges in the six regular 4-dimensional polytopes.
 * https://oeis.org/A063926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = edges of regular 4‑polytopes, in order of increasing
 *            3‑dimensional faces. Known values: a(1)=10, a(2)=32, a(3)=24,
 *            a(4)=96, a(5)=1200, a(6)=720. For n>6 the sequence has no
 *            defined terms, so return 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[6] = {10, 32, 24, 96, 1200, 720};
    if (n <= 6)
        return table[n - 1];
    return 0; /* undefined indices */
}
