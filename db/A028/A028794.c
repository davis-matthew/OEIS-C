/*
 * A028794  Nonsquares mod 81.
 * https://oeis.org/A028794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth positive integer that is not a quadratic residue
 *            modulo 81
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precomputed table of quadratic residues modulo 81. */
static bool squares[81];
static bool squares_initialized = false;

static void initSquares(void)
{
    for (int i = 0; i < 81; ++i) squares[i] = false;
    for (int k = 0; k < 81; ++k)
    {
        int r = (k * k) % 81;
        squares[r] = true;
    }
    squares_initialized = true;
}

seq_t a(idx_t n)
{
    if (!squares_initialized) initSquares();

    idx_t count = 0;
    idx_t num   = 0;
    int mod     = 0;

    while (count < n)
    {
        ++num;
        ++mod;
        if (mod == 81) mod = 0;
        if (!squares[mod])
        {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
    /* Unreachable, but return a value to satisfy the compiler. */
    return (seq_t)0;
}
