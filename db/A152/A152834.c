/*
 * A152834  Numbers of the form 6*k+1 that use only digits 2 and 3.
 * https://oeis.org/A152834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 340, u64 n <= 340,
 *            u128 n <= 340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A152834 by brute‑force enumeration:
   For each length L generate all strings of 2/3 ending with 3,
   keep those where the number of 2's is ≡ 2 (mod 3),
   and return the n‑th such number. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t remaining = n;
    for (idx_t L = 3; ; ++L) {
        idx_t k = L - 1;                     /* positions before the trailing 3 */
        unsigned long long maxMask = 1ULL << k;
        for (unsigned long long mask = 0; mask < maxMask; ++mask) {
            idx_t twos = k - __builtin_popcountll(mask);
            if ((twos % 3) == 2) {           /* keep only valid patterns */
                if (remaining == 1) {
                    uint64_t val = 0;
                    for (idx_t i = 0; i < k; ++i) {
                        unsigned long long bit = (mask >> (k - 1 - i)) & 1ULL;
                        val = val * 10 + (bit ? 3 : 2);
                    }
                    val = val * 10 + 3;       /* append the final 3 */
                    return (seq_t)val;
                }
                --remaining;
            }
        }
    }
}
