/*
 * A055691  Numbers k such that 8^k == -1 (mod k-1).
 * https://oeis.org/A055691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds k such that 8^k ≡ -1 (mod k-1).
   k must be even, and all solutions satisfy k ≡ 2 (mod 4). */

/* Utility for modular exponentiation */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for already found values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next candidate k to test (only k ≡ 2 (mod 4)) */
    static idx_t test_k = 2;

    /* grow cache when needed */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) { /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* search until we have at least n terms stored */
    while (cache_len < (size_t)n) {
        seq_t mod = (seq_t)(test_k - 1);
        seq_t t = powmod((seq_t)8, (uint64_t)test_k, mod);

        /* check 8^k ≡ -1 (mod k-1) ⇔ t = mod - 1 */
        if (t == mod - (seq_t)1) {
            cache[cache_len++] = (seq_t)test_k;
        }
        test_k += 4;            /* next candidate (k ≡ 2 mod 4) */
    }

    return cache[n - 1];
}
