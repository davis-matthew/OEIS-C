/*
 * A160797  First differences of A160796.
 * https://oeis.org/A160797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 3^(popcount(n)-1) if n is odd; a(n) = 7 *
 *            3^(v2(n)+popcount(n)-2) if n is even. popcount(n) = number of
 *            1‑bits in the binary expansion of n. v2(n) = exponent of 2
 *            dividing n (number of trailing zeros)
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The offset is 1, but the implementation works for n >= 1. */
    if (n == 0)
        return 0;                       /* undefined for the sequence, but safe */

    /* use GCC/Clang built‑ins for speed and correctness on 64‑bit values */
    idx_t pc = __builtin_popcountll((unsigned long long)n); /* popcount */

    if ((n & 1ULL) == 0) {              /* n even */
        idx_t v2 = __builtin_ctzll((unsigned long long)n); /* trailing zeros */
        idx_t exp = v2 + pc - 2;        /* exponent of 3 */
        seq_t pow3 = seq_pow((seq_t)3, exp);
        return pow3 * (seq_t)7;
    } else {                            /* n odd */
        idx_t exp = pc - 1;             /* exponent of 3 */
        return seq_pow((seq_t)3, exp);
    }
}
