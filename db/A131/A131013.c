/*
 * A131013  Ten-digit numbers that contain no repeated digits and are
 *          divisible by every integer from 2 through 18.
 * https://oeis.org/A131013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, nonn, fini
 * Formula:   none – a(n) is the nth 10‑digit permutation of 0–9, with no
 *            leading zero, that is divisible by every integer from 2 to 18
 *            (i.e., divisible by lcm(2 … 18) = 12252240)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   generate_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/generate_perm.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* static cache of all sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static bool initialized = false;

    if (!initialized) {
        generate_perm(0, (seq_t)0, 0, &cache, &cache_size, &cache_cap);
        initialized = true;
    }

    if ((idx_t)cache_size < n) return (seq_t)0;
    return cache[(size_t)n - 1];
}
