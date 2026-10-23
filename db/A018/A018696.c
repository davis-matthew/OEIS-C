/*
 * A018696  Divisors of 874.
 * https://oeis.org/A018696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 874 in increasing order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This does not hard‑code the divisor list; it generates all 8 divisors
 * from the prime factorisation 874 = 2 × 19 × 23, sorts them,
 * and returns the n-th element (1‑based). */

#include <stddef.h>
#include <stdbool.h>

static bool generated = false;
static seq_t divs[8];

static void generate_divisors(void)
{
    /* Factorisation: 874 = 2^1 × 19^1 × 23^1. */
    const uint64_t p1 = 2,  p2 = 19,  p3 = 23;
    /* enumerate all combinations of exponents 0 or 1 */
    int pos = 0;
    for (int e1 = 0; e1 <= 1; ++e1)
        for (int e2 = 0; e2 <= 1; ++e2)
            for (int e3 = 0; e3 <= 1; ++e3)
            {
                seq_t val = 1;
                if (e1) val *= p1;
                if (e2) val *= p2;
                if (e3) val *= p3;
                divs[pos++] = val;
            }

    /* sort the 8 values ascending (insertion sort is trivial for 8 items) */
    for (int i = 1; i < 8; ++i)
    {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key)
        {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    generated = true;
}

seq_t a(idx_t n)
{
    if (!generated)
        generate_divisors();

    if (n < 1ULL || n > 8ULL)
        return 0;           /* sequence terminates after 8 terms */
    return divs[n - 1];
}
