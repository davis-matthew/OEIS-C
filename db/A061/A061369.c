/*
 * A061369  a(n) = smallest square in the arithmetic progression {n*k+1 : k >
 *          0}.
 * https://oeis.org/A061369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{x^2 : x^2 = n*k+1, k>0}
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;
    idx_t n_idx = (idx_t)n;
    for (idx_t m = 2;; ++m)
    {
        unsigned long long msq = (unsigned long long)m * m;
        if ((msq % n_idx) == 1ULL)
            return (seq_t)msq;
    }
}
