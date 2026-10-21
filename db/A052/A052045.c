/*
 * A052045  Cubes lacking the digit zero in their decimal expansion.
 * https://oeis.org/A052045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th cube that contains no zero digit (decimal)
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* True if the decimal representation of x contains a digit 0. */
static bool has_zero_digit(seq_t x)
{
    while (x > 0) {
        if ((x % 10) == 0)
            return true;
        x = x / 10;
    }
    return false;
}

/* Compute the n-th term of A052045 for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* indices below the offset are undefined */

    idx_t k = 0;                   /* integer whose cube is being tested */
    idx_t count = 0;               /* how many valid cubes found so far */
    seq_t result = (seq_t)0;       /* value to return */

    while (count < n) {
        ++k;
        seq_t sk = (seq_t)k;       /* promote to seq_t to avoid native overflow */
        seq_t cube = sk * sk * sk; /* compute k³ with seq_t arithmetic */

        if (!has_zero_digit(cube)) {
            ++count;
            result = cube;
        }
    }

    return result;
}
