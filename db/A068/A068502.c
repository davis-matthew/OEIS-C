/*
 * A068502  Composite numbers k such that gcd(sigma(k), k) = gcd(k, phi(k)).
 * https://oeis.org/A068502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 8719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, gcd, is_prime, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for composite numbers k such that
   gcd(sigma(k), k) = gcd(k, phi(k))   (OEIS A068502)
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* values already found */
    static size_t cache_len = 0;    /* number of terms stored   */
    static size_t cache_cap = 0;    /* allocated capacity       */
    static idx_t  next      = 4;    /* next integer to test     */

    /* generate terms until we have at least n values */
    while (cache_len < n) {
        if (next >= 4 && !is_prime(next)) {
            seq_t sigma   = sigma_of(next);
            seq_t phi_val = phi(next);
            seq_t g1      = gcd(sigma, (seq_t)next);
            seq_t g2      = gcd((seq_t)next, phi_val);
            if (g1 == g2)
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next);
        }
        ++next;
    }
    return cache[n - 1];
}
