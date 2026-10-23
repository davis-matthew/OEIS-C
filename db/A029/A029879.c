/*
 * A029879  Binomial transform of Thue-Morse sequence A001285.
 * https://oeis.org/A029879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} binom(n,k)*(1 + (popcount(k) mod 2)) where
 *            popcount(k) is the number of 1‑bits in the binary expansion of k
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom   = 1;               // binom(n,0)
    for (idx_t k = 0; k <= n; ++k) {
        /* Thue–Morse value t(k) = popcount(k) mod 2 (0 or 1) */
        unsigned int t = __builtin_popcountll((uint64_t)k) & 1u;
        /* add (1 + t) * binom(n,k) */
        seq_t addend = binom;
        if (t) addend = addend + binom;   /* multiply by 2 when t==1 */
        result = result + addend;

        /* update binom for next k: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        if (k < n) {
            seq_t num = binom * ((seq_t)(n - k));
            seq_t den = (seq_t)(k + 1);
            binom = num / den;
        }
    }
    return result;
}
