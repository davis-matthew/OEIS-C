/*
 * A050266  Primes of the form n^3 + n^2 + 17, for nonnegative values of n.
 * https://oeis.org/A050266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form n^3 + n^2 + 17 for n ≥ 0. a(1)
 *            corresponds to n = 0, a(2) to n = 1, etc
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 501, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: a(1) is the first term */

    /* static cache – enough for n up to about 10 000 (guaranteed in tests) */
    enum { MAX_CACHE = 10010 };
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;
    static idx_t last_n_checked = 0;      /* next n to test, starting at 0 */

    /* generate primes until the cache contains at least n primes */
    while (cache_len < n) {
        seq_t nn  = (seq_t)last_n_checked;                 /* current n */
        seq_t val = nn * nn * nn + nn * nn + (seq_t)17;     /* n³ + n² + 17 */

        if (isPrimeGeneric(val))
            cache[cache_len++] = val;                       /* store prime */

        last_n_checked++;                                   /* next n */
    }

    return cache[n - 1];
}
