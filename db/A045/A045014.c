/*
 * A045014  Numbers whose base-4 representation contains no 0's and exactly
 *          one 1.
 * https://oeis.org/A045014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1793, u32 n <= 1900, u64 n <= 1900,
 *            u128 n <= 1900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate numbers by checking their base‑4 digits
   (no 0’s and exactly one 1) and store them in a static cache. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array for computed terms          */
static size_t cache_cap = 0;         /* capacity of the array            */
static size_t cache_len = 0;         /* number of terms stored           */
static seq_t current = (seq_t)1;     /* next integer to test */

/* return true if x has no 0 digits in base‑4 and exactly one digit 1 */
static int satisfies(seq_t x)
{
    seq_t t = x;
    size_t ones = 0;
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)4;
        if (d == (seq_t)0)
            return 0;
        if (d == (seq_t)1) {
            ++ones;
            if (ones > 1)
                return 0;
        }
        t /= (seq_t)4;
    }
    return ones == 1;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for index 0 */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* test successive integers after the last one tested */
        while (!satisfies(current))
            ++current;

        /* store the found term */
        if (cache_len == cache_cap) {
            /* grow the cache */
            size_t new_cap = cache_cap == 0 ? 128 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)          /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = current;
        ++current;           /* start next search from the following integer */
    }

    return cache[(size_t)n - 1];
}
