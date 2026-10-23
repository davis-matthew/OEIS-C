/*
 * A197768  Ceiling((8*n+1/n)^n).
 * https://oeis.org/A197768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((8*n + 1/n)^n) = ceil((8*n^2+1)^n / n^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. Treat n = 0 gracefully. */
    if (n == 0) return 0;

    /* Promote the index to seq_t once to avoid mixed‑type arithmetic. */
    seq_t NN = (seq_t)n;

    /* (8*n^2 + 1) fits in a seq_t for all n used in the tests. */
    seq_t C = (seq_t)8 * NN * NN + 1;

    /* For small (built‑in) seq_t types use 128‑bit intermediate
       arithmetic which is sufficient for the 64‑bit build up to n = 10.
       The compiler will still compile the big‑int code for other builds
       but the branch will be avoided at run time. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 num = 1;
        unsigned __int128 den = 1;

        for (idx_t i = 1; i <= n; ++i) {
            num *= (unsigned __int128)C;
            den *= (unsigned __int128)NN;
        }

        unsigned __int128 res = (num + den - 1) / den;
        return (seq_t)res;
    } else {
        /* For arbitrary‑precision builds, rely on the supplied big_pow. */
        seq_t num = big_pow(C, n);
        seq_t den = big_pow(NN, n);
        seq_t res = (num + den - 1) / den;
        return res;
    }
}
