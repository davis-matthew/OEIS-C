/*
 * A130665  a(n) = Sum_{k=0..n} 3^wt(k), where wt() = A000120().
 * https://oeis.org/A130665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Sum_{k=0..n} 3^{wt(k)}, where wt(k) is the Hamming weight
 *            (popcount) of k
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) for arbitrary precision or 64‑bit seq_t. */
seq_t a(idx_t n)
{
    /* Precompute powers of 3 up to 64 bits of Hamming weight. */
    static seq_t pow3[65];
    static bool init = false;
    if (!init) {
        pow3[0] = (seq_t)1;
        for (int i = 1; i <= 64; ++i)
            pow3[i] = pow3[i-1] * (seq_t)3;
        init = true;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; ; ++k) {
        /* popcount of k using GCC/Clang builtin. */
        int w = __builtin_popcountll((unsigned long long)k);
        sum += pow3[w];
        if (k == n)
            break;
    }
    return sum;
}
