/*
 * A008558  Repeatedly convert from decimal to octal.
 * https://oeis.org/A008558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = f^{n-1}(9) where f(x) = integer obtained by interpreting
 *            the octal representation of x as a decimal number. This is a
 *            recurrence defined by a(1) = 9, a(n) = f(a(n-1))
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t step(seq_t x)
{
    seq_t result = 0;
    seq_t multiplier = 1;
    while (x > 0)
    {
        seq_t digit = x % 8;
        x = x / 8;
        result += digit * multiplier;
        multiplier *= 10;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Undefined for n < 1; return 0 as placeholder. */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Compute values up to index n. */
    while (len < (size_t)n)
    {
        seq_t val = (len == 0) ? (seq_t)9 : step(cache[len - 1]);

        if (len >= cap)
        {
            size_t new_cap = (cap == 0) ? 8 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();   /* Allocation failed. */
            cache = tmp;
            cap = new_cap;
        }

        cache[len] = val;
        ++len;
    }

    return cache[n - 1];
}
