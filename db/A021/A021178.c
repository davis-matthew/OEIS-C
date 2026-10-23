/*
 * A021178  Decimal expansion of 1/174.
 * https://oeis.org/A021178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 174) mod 10 (digit extraction by
 *            long‑division of 1/174)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *digits = NULL;
static size_t cache_len = 0;   /* number of digits currently stored   */
static size_t cache_cap = 0;   /* allocated capacity of the array      */
static unsigned long long rem = 1ULL; /* remainder after the last generated digit */

seq_t a(idx_t n)
{
    /* Extend the cache if necessary */
    if ((idx_t)cache_len <= n) {
        size_t need = (size_t)n + 1;

        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* out of memory */
            digits = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_len; i < need; ++i) {
            rem *= 10ULL;                     /* bring down next digit */
            seq_t digit = (seq_t)(rem / 174ULL); /* quotient 0–9 */
            rem %= 174ULL;                    /* new remainder   */
            digits[i] = digit;
        }
        cache_len = need;
    }

    return digits[(size_t)n];
}
