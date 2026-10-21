/*
 * A240162  Tower of 3's modulo n.
 * https://oeis.org/A240162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^a(φ(n)) mod n
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; this case is unused */
    if (n == 1) return 0;      /* a(1) = 0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static seq_t undef = (seq_t)-1;   /* sentinel for “uncomputed” */

    /* Expand cache if necessary */
    if (n >= cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = undef;
    }

    /* Return cached value if already computed */
    if (cache[n] != undef) return cache[n];

    /* Compute Euler's totient φ(n) */
    seq_t phi_seq = calc_phi((seq_t)n);
    idx_t phi = (idx_t)phi_seq;   /* φ(n) < n, so fits in idx_t */

    /* Recursive call for a(φ(n)) */
    seq_t a_phi = a(phi);

    /* Modulo power: 3^a_phi (mod n) */
    seq_t result = mod_pow((seq_t)3, (uint64_t)a_phi, (seq_t)n);

    /* Cache and return */
    cache[n] = result;
    return result;
}
