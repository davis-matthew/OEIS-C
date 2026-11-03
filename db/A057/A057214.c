/*
 * A057214  Numbers k such that k | 10^k + 9^k + 8^k + 7^k.
 * https://oeis.org/A057214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057214: numbers k such that k | 10^k + 9^k + 8^k + 7^k */
/* The routine checks each integer k in increasing order and stores
   the ones that satisfy the divisibility condition in a growing
   cache.  The function returns the (n‑1)-indexed element of that
   cache. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

static bool good(idx_t k)
{
    seq_t mod_k = (seq_t)k;
    seq_t r10 = powmod(10, (uint64_t)k, mod_k);
    seq_t r9  = powmod(9,  (uint64_t)k, mod_k);
    seq_t r8  = powmod(8,  (uint64_t)k, mod_k);
    seq_t r7  = powmod(7,  (uint64_t)k, mod_k);

    seq_t sum = r10 + r9;
    if (sum >= mod_k) sum -= mod_k;
    sum += r8;
    if (sum >= mod_k) sum -= mod_k;
    sum += r7;
    if (sum >= mod_k) sum -= mod_k;

    return sum == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t cur_k  = 1;
    static bool  init   = false;

    if (!init) {
        init = true;
        cap   = 64;
        cache = mem_new(seq_t, cap);
    }

    while (len < n) {
        if (good(cur_k)) {
            if (len == cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = (seq_t)cur_k;
        }
        ++cur_k;
    }

    return cache[n-1];
}
