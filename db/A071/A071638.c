/*
 * A071638  Numbers n such that phi(n) = b(n,1)^b(n,0) where b(n,1) is the
 *          number of 1's in binary representation of n and b(n,0) the number
 *          of 0's.
 * https://oeis.org/A071638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m such that φ(m) = b(m,1)^b(m,0) where
 *            b(m,1) is the number of 1‑bits in the binary representation of m
 *            and b(m,0) the number of 0‑bits. Indices start at 1
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   cache_push, phi, pow_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/cache_push.h"
#include "../utils/pow_limit.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* The sequence function.  It lazily generates terms and caches them.
   Because the caller may request terms in any order, caching ensures
   each term is evaluated only once. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t len   = 0;      /* number of cached terms */
    static size_t cap   = 0;      /* capacity of cache array */
    static idx_t next_candidate = 1;  /* next integer to test */

    /* The sequence is defined for n >= 1 only. */
    if (n == 0) return 0;

    /* Build terms until we have at least n of them. */
    while (len < (size_t)n) {
        idx_t x = next_candidate++;
        /* Count bits in binary representation of x. */
        unsigned bits  = 64 - __builtin_clzll((unsigned long long)x);
        unsigned ones  = __builtin_popcountll((unsigned long long)x);
        unsigned zeros = bits - ones;

        seq_t phi_val = phi(x);                       /* φ(x) from utils */
        seq_t pow_val = pow_limit((seq_t)ones, zeros, phi_val);

        if (pow_val == phi_val) {
            cache_push((seq_t)x, &cache, &len, &cap);
        }
    }
    return cache[n - 1];
}
