/*
 * A059244  Primes p such that x^19 = 2 has no solution mod p.
 * https://oeis.org/A059244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 349, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 848 (2026-11-03)
 * Helpers:   mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059244: primes p for which x^19 ≡ 2 (mod p) has no solution.
   The algorithm scans primes p > 2 using next_prime.  If p ≡ 1 (mod 19)
   we compute e = (p-1)/19 and test whether 2^e ≡ 1 (mod p).  If not, p
   is recorded as a term.  The first n terms are cached in a static array
   and returned on demand. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t last_checked = 1;   /* last prime processed; start at 1 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */

    /* grow the cache until we have n elements */
    while (cache_len < (size_t)n)
    {
        last_checked = next_prime(last_checked);  /* next prime > last_checked */

        /* only p ≡ 1 (mod 19) can fail to have a 19th‑root of 2 */
        if (last_checked > 2 && ((last_checked - 1) % 19) == 0)
        {
            uint64_t e = (last_checked - 1) / 19;
            seq_t mod = (seq_t)last_checked;
            seq_t r   = powmod((seq_t)2, e, mod);

            if (r != (seq_t)1)   /* no 19th‑power solution */
            {
                /* ensure capacity */
                if (cache_len + 1 > cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();   /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
