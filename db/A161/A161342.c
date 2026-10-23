/*
 * A161342  Number of "ON" cubic cells at n-th stage in simple 3-dimensional
 *          cellular automaton: a(n) = A160428(n)/8.
 * https://oeis.org/A161342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} 7^{A000120(k)}
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 2007, u64 n <= 9081,
 *            u128 n <= 9081, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (where A000120(k) is the popcount of k) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Pre‑compute powers of 7 up to 64 (sufficient for 64‑bit indices) */
    seq_t pow7[65];
    pow7[0] = 1;
    for (int i = 1; i < 65; ++i) {
        pow7[i] = pow7[i - 1] * (seq_t)7;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        /* popcount of k using GCC/Clang built‑in */
        unsigned pc = __builtin_popcountll((unsigned long long)k);
        sum += pow7[pc];
    }
    return sum;
}
