/*
 * A197910  Ceiling((n+1/2)^n).
 * https://oeis.org/A197910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ceil((n+1/2)^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined below the offset, but harmless */
        return 0;

    /* base = 2*n + 1 (as seq_t) */
    seq_t base = (seq_t)(2ULL * (seq_t)n + 1ULL);

    /* For small seq_t types and modest n, use 128‑bit arithmetic
       to avoid overflow of the numerator. */
    if (sizeof(seq_t) <= 8 && n <= 127) {
        unsigned __int128 b = (unsigned __int128)(2ULL * (seq_t)n + 1ULL);
        unsigned __int128 num = 1;
        for (idx_t i = 0; i < n; ++i)
            num *= b;

        unsigned __int128 denom = ((unsigned __int128)1) << n;
        /* ceil(num / denom) = (num + denom - 1) / denom */
        unsigned __int128 up = num + denom - 1;
        return (seq_t)(up / denom);
    }

    /* General case: use arbitrary‑precision helper functions. */
    seq_t num   = big_pow(base, (seq_t)n);   /* (2n+1)^n  */
    seq_t denom = pow2(n);                 /* 2^n       */
    /* ceil(num / denom) = (num + denom - 1) / denom */
    seq_t up = num + denom - 1;
    return up / denom;
}
