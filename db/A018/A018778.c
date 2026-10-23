/*
 * A018778  Divisors of 1017.
 * https://oeis.org/A018778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 1017 in ascending order (defined only
 *            for n = 1 … 6)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t divs[6];
static idx_t divcnt = 0;
static bool initialized = false;

static void init_divisors(void)
{
    const idx_t limit = (idx_t)1017;
    idx_t i, k = 0;

    for (i = 1; i <= limit; ++i)
    {
        if (limit % i == 0)
        {
            divs[k++] = (seq_t)i;
        }
    }
    divcnt = k;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_divisors();

    if (n < 1 || n > divcnt)
        return (seq_t)0;
    return divs[n - 1];
}
