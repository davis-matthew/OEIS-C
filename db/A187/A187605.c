/*
 * A187605  Primes of the form k^k + k - 1.
 * https://oeis.org/A187605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form k^k + k - 1, for the n‑th such k (OEIS
 *            A187605)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined; harmless for n >= 1 */

    idx_t found = 0;
    idx_t k = 1;

    for (;;) {
        seq_t ks = (seq_t)k;          /* convert k to seq_t */
        seq_t val = big_pow(ks, ks);  /* ks^ks   */
        val += ks;                    /* + k     */
        val -= 1;                     /* - 1     */

        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;          /* desired prime */
        }
        ++k;                          /* next k */
    }
}
