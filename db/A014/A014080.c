/*
 * A014080  Factorions: equal to the sum of the factorials of their digits in
 *          base 10 (cf. A061602).
 * https://oeis.org/A014080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for all factorions up to 7*9! = 2540160 */
#include <stddef.h>
#include <stdbool.h>

/* Pre‑computed factorials of digits 0..9 */
static const seq_t _fact[10] = {1,1,2,6,24,120,720,5040,40320,362880};

/* Cache of the found factorions */
static seq_t    cache[10];
static size_t   cache_len   = 0;
static bool     cache_done  = false;

/* Build the cache once */
static void _build_cache(void)
{
    if (cache_done) return;
    cache_done = true;

    /* Upper bound: maximum sum of factorials for any decimal digit string */
    const idx_t upper = 2540160; /* 7 * 9! */

    for (idx_t x = 1; x <= upper; ++x) {
        seq_t sum = 0;
        idx_t t = x;
        while (t) {
            unsigned d = t % 10;
            sum += _fact[d];
            t /= 10;
        }
        if (sum == (seq_t)x) {
            cache[cache_len++] = x;
        }
    }
}

/* Return the nth factorion (1‑based).  Indices beyond the known terms
   return 0. */
seq_t a(idx_t n)
{
    _build_cache();
    if (n >= 1 && n <= cache_len)
        return cache[n-1];
    return 0;
}
