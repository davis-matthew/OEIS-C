/*
 * A072750  Counting factor 7 in the first n squarefree numbers.
 * https://oeis.org/A072750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of the first n square‑free integers that are
 *            divisible by 7
 * Range:     u8 n <= 2037, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether an integer x is square‑free: no prime squared divides x.
   The routine uses only trial division up to sqrt(x).                */
static bool is_squarefree(idx_t x)
{
    if (x <= 1) return true;
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            /* p divides x; if p² also divides x, then x is not square‑free */
            if (x % (p * p) == 0) return false;
        }
    }
    return true;
}

/* Return a(n) for the OEIS sequence A072750.                          */
seq_t a(idx_t n)
{
    seq_t count = 0;       /* number of first n square‑free numbers
                               that are multiples of 7                  */
    idx_t seen  = 0;       /* how many square‑free numbers have been seen */
    for (idx_t x = 1; seen < n; ++x)
    {
        if (is_squarefree(x))
        {
            ++seen;
            if (x % 7 == 0) ++count;
        }
    }
    return count;
}
