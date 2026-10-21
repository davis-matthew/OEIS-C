/*
 * A085219  Array A(x,y): "rised concatenation" of factorial expansions of x &
 *          y, listed antidiagonalwise as A(0,0), A(1,0), A(0,1), A(2,0),
 *          A(1,1), A(0,2), ... Zero is expanded as an empty string.
 * https://oeis.org/A085219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factorial_digits, index_to_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085219: rised concatenation of factorial expansions */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial_digits.h"
#include "../utils/index_to_xy.h"

/* Pre‑compute factorials up to 40! (enough for n <= 10^4). */
static seq_t fac[41];
static void init_factorials(void)
{
    static bool once = false;
    if (once) return;
    once = true;
    fac[0] = 1; /* unused */
    fac[1] = (seq_t)1;
    for (idx_t i = 2; i <= 40; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;
}

/* Compute A(x,y) as defined in the OEIS entry. */
static seq_t A_xy(idx_t x, idx_t y)
{
    idx_t dx, dy;
    idx_t digits_x[32], digits_y[32];

    factorial_digits(x, digits_x, &dx);
    factorial_digits(y, digits_y, &dy);

    init_factorials();

    seq_t msd = 0;
    if (dy > 0)
        msd = (seq_t)digits_y[dy - 1];

    seq_t result = 0;

    /* y part: least significant first */
    for (idx_t pos = 1; pos <= dy; ++pos) {
        result += (seq_t)digits_y[pos - 1] * fac[pos];
    }

    /* x part (modified) occupies the higher factorial positions */
    for (idx_t pos = 1; pos <= dx; ++pos) {
        seq_t coeff = (seq_t)digits_x[pos - 1] + msd;
        result += coeff * fac[dy + pos];
    }

    return result;
}

seq_t a(idx_t n)
{
    idx_t x, y;
    index_to_xy(n, &x, &y);
    return A_xy(x, y);
}
