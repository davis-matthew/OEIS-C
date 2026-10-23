/*
 * A193343  a(1) = 42; a(n) = 10*(sum of digits of a(n-1)) + (last digit of
 *          a(n-1)) + 1 for n >= 2.
 * https://oeis.org/A193343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=42; a(n)=10*sum_of_digits(a(n-1)) + last_digit(a(n-1)) + 1
 *            for n>=2
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


/* Period of the sequence (a(91)=a(1)) */
#define PERIOD 90

/* Static cache and initialization flag */
static seq_t cache[PERIOD];
static int initialized = 0;

/* Compute the next value from the previous one */
static seq_t next_value(seq_t prev)
{
    seq_t sum = 0;
    seq_t v   = prev;
    /* sum of the digits of prev */
    while (v)
    {
        sum += v % 10;
        v   /= 10;
    }
    seq_t last = prev % 10;
    return 10 * sum + last + 1;
}

/* Pre‑compute one full period */
static void init_cache(void)
{
    cache[0] = 42;                     /* a(1) */
    for (size_t i = 1; i < PERIOD; ++i)
        cache[i] = next_value(cache[i - 1]);
    initialized = 1;
}

/* External entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* Undefined for index 0, return 0 for safety */
    if (!initialized)
        init_cache();

    size_t idx = ((size_t)(n - 1)) % PERIOD;
    return cache[idx];
}
