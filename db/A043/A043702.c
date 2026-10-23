/*
 * A043702  Numbers whose base-13 representation has an even number of runs.
 * https://oeis.org/A043702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by iterating integers and selecting those with an even number of runs in base‑13. */
#include <stdint.h>
#include <stdbool.h>

/* Return true if x has an even number of runs in its base‑13 representation. */
static bool even_runs(idx_t x)
{
    /* Extract base‑13 digits.  The array is large enough for all values that
       appear for n up to at least 10000. */
    uint8_t dig[64];
    int len = 0;
    while (x > 0)
    {
        dig[len++] = (uint8_t)(x % 13);
        x /= 13;
    }

    /* Numbers with less than two digits have one run (odd), so skip them. */
    if (len <= 1)
        return false;

    /* Count the transitions between consecutive digits.  A run count is
       changes + 1.  Even runs mean an odd number of changes. */
    int changes = 0;
    for (int i = len - 1; i > 0; --i)
    {
        if (dig[i] != dig[i - 1])
            ++changes;
    }
    return (changes & 1) != 0; /* changes odd => even run count */
}

/* External entry point for the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset < 1, but harmless */

    idx_t val   = 13;   /* first number with two base‑13 digits */
    idx_t count = 0;

    while (1)
    {
        if (even_runs(val))
        {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
        ++val;
    }
}
