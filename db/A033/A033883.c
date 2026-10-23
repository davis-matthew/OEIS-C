/*
 * A033883  Deficiency of the deficient or perfect numbers: m = 2n - sigma(n)
 *          for n such that m >= 0.
 * https://oeis.org/A033883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2k – σ(k) for the (n+1)-th integer k ≥ 1 * such that 2k –
 *            σ(k) ≥ 0, enumerated in increasing k * (a(0)=1, a(1)=1, a(2)=2,
 *            …)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    idx_t target = n;                  /* how many admissible numbers to skip */
    for (idx_t k = 1; ; ++k) {
        seq_t sigma = sigma_of(k);          /* σ(k) */
        /* use unsigned __int128 for safe arithmetic on 64‑bit k */
        unsigned __int128 twok   = (unsigned __int128)k << 1;
        unsigned __int128 sig128 = (unsigned __int128)sigma;
        if (twok >= sig128) {               /* k is deficient or perfect */
            unsigned __int128 def128 = twok - sig128;
            if (target == 0)
                return (seq_t)def128;       /* found the requested term */
            --target;                       /* skip this one */
        }
    }
}
