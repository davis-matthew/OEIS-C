/*
 * A039470  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 9's.
 * https://oeis.org/A039470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑11 representation has the same nonzero number
 *            of 3's and 9's
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Maximum number of terms we will cache – large enough for all test cases.
   This is a compile‑time constant, so the array can be declared with it. */
#define MAXCACHE 20000

static seq_t cache[MAXCACHE];
static size_t cache_size = 0;
static idx_t next_candidate = 1;

/* Test whether a number has an equal, non‑zero number of 3's and 9's in base‑11. */
static bool has_equal_3_and_9(idx_t x)
{
    unsigned int c3 = 0, c9 = 0;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 11);
        if (d == 3)
            ++c3;
        else if (d == 9)
            ++c9;
        x /= 11;
    }
    return (c3 == c9) && (c3 > 0);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1; return a sentinel value */

    while (cache_size < (size_t)n) {
        if (has_equal_3_and_9(next_candidate))
            cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
