/*
 * A177245  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 3.
 * https://oeis.org/A177245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth Fibonacci number whose decimal expansion does not
 *            contain any digit 3. The function builds the sequence on demand,
 *            caching the results so that repeated calls are fast. Sequence
 *            stops at the 223th term (Fib(223)), after which no further
 *            Fibonacci numbers have this property
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"   /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */

#define MAX_FOUND 223   /* OEIS A177245 has exactly 223 terms */

/* Cache storage */
static seq_t cache[MAX_FOUND];
static size_t cache_len = 0;   /* number of terms currently stored */

/* Current Fibonacci state */
static seq_t f0 = 0;   /* F(k)   */
static seq_t f1 = 1;   /* F(k+1) */

/* Return true iff decimal representation of x contains no digit 3. */
static bool has_no_three(seq_t x)
{
    uint8_t buf[256];
    size_t len = 0;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        if (buf[i] == 3)
            return false;
    return true;
}

/* Append a new term to the cache. Assumes cache_len < MAX_FOUND. */
static void add_to_cache(seq_t v)
{
    cache[cache_len++] = v;
}

/* Prepare cache up to at least 'target' terms, up to MAX_FOUND. */
static void ensure_cache(idx_t target)
{
    if (target > MAX_FOUND)
        target = MAX_FOUND;

    while (cache_len < (size_t)target && cache_len < (size_t)MAX_FOUND)
    {
        if (has_no_three(f0))
        {
            /* Skip duplicate Fibonacci numbers (only 1 repeats). */
            if (cache_len == 0 || f0 != cache[cache_len - 1])
                add_to_cache(f0);
        }

        /* Advance to next Fibonacci number. */
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > MAX_FOUND)
        return 0;   /* undefined for out-of-range indices */

    ensure_cache(n);
    return cache[n - 1];
}
