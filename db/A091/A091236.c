/*
 * A091236  Nonprimes of form 4k+3.
 * https://oeis.org/A091236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091236: Nonprimes of the form 4k+3.
   a(n) = the nth composite integer congruent to 3 (mod 4). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t cache[30000];       /* cached terms (enough for n ≈ 10000) */
static idx_t cache_len = 0;      /* number of terms currently cached */
static idx_t next_candidate = 3; /* next 4k+3 number to test */

/* Generate terms until we have at least n terms cached. */
static void fill_cache(idx_t n)
{
    while (cache_len < n) {
        if (!is_prime_big((seq_t)next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += (idx_t)4;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0; harmless */

    if (cache_len < n) fill_cache(n);
    return cache[n - 1];
}
