/*
 * A221740  a(n) = -4*((n-1)*(n+1)^(n+1)+1)/(((-1)^n-3)*n^3).
 * https://oeis.org/A221740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -4*((n-1)*(n+1)^(n+1)+1)/(((-1)^n-3)*n^3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* use a fast path with built‑in 128‑bit arithmetic if the target
       sequence type is 8 bytes or smaller; otherwise fall back to the
       arbitrary‑precision big_pow routine. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 base = nn + 1;
        unsigned __int128 pow  = 1;
        for (idx_t i = 0; i <= n; ++i)
            pow *= base;

        unsigned __int128 numer = (nn - 1) * pow + 1;
        unsigned __int128 denom = nn * nn * nn;
        unsigned __int128 res   = (n & 1u) ? (numer / denom)
                                          : ((numer * 2) / denom);
        return (seq_t)res;
    } else {
        seq_t nseq = (seq_t)n;
        seq_t base = nseq + 1;
        seq_t pow  = big_pow(base, nseq + 1);

        seq_t numer = (nseq - 1) * pow + 1;
        seq_t denom = nseq * nseq * nseq;
        seq_t res   = (n & 1u) ? (numer / denom)
                               : ((numer * 2) / denom);
        return res;
    }
}
