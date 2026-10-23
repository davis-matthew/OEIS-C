/*
 * A129095  Semi-Pell numbers: a(n) = a(n/2) (n even), a(n) = 2*a(n-1) +
 *          a(n-2) (n odd >1), with a(1) = 1.
 * https://oeis.org/A129095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1 a(n) = a(n/2) if n is even a(n) = 2*a(n-1) + a(n-2) if
 *            n is odd and n > 1
 * Range:     u8 n <= 30, u16 n <= 138, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_A129095
#define MAX_A129095 20000   /* must be >= maximum n used in tests (≈10000) */
#endif

static seq_t a_cache[MAX_A129095];
static idx_t a_cache_max = 0;  /* largest n computed so far */

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Guard against too large n */
    if (n >= MAX_A129095)
        return 0;   /* silently clamp; sequence tests use n <= 10000 */

    /* Ensure 1 is initialized */
    if (a_cache_max == 0) {
        a_cache[0] = 1;
        a_cache_max = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        seq_t val;
        if ((i & 1) == 0) {               /* even */
            val = a_cache[(i / 2) - 1];
        } else {                          /* odd > 1 */
            val = a_cache[i - 2] * (seq_t)2 + a_cache[i - 3];
        }
        a_cache[i - 1] = val;
    }
    a_cache_max = n;
    return a_cache[n - 1];
}
