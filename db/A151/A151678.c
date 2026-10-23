/*
 * A151678  a(n) = sum_{k >= 0} binomial(wt(n+k),2*k), where wt() = A000120().
 * https://oeis.org/A151678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k\ge 0} \binom{\text{wt}(n+k)}{\,2k\,}
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  wt(x) = number of 1‑bits in binary representation of x (A000120).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t m   = n + k;
        seq_t wt  = (seq_t)__builtin_popcountll((uint64_t)m);
        /* when 2*k > wt no further binomial terms are non‑zero. */
        if ((seq_t)(2 * k) > wt) {
            break;
        }
        sum += binom((idx_t)wt, 2 * k);
    }
    return sum;
}
