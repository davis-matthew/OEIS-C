/*
 * A182346  Primes of the form n^4 + 6.
 * https://oeis.org/A182346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form k^4 + 6 (k ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_n   = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    while (cache_len < (size_t)n)
    {
        last_n++;
        seq_t m = (seq_t)last_n; /* compute m^4 + 6 */
        seq_t v = m * m;         /* m^2                    */
        v = v * v;               /* m^4                    */
        v += (seq_t)6;           /* m^4 + 6                */

        if (isPrimeGeneric(v))
            append_candidate(&cache, &cache_len, &cache_cap, v);
    }

    return cache[(size_t)n - 1];
}
