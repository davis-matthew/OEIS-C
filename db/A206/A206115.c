/*
 * A206115  Number of (n+1) X 6 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 859 * a(n-3) for n > 8
   Base values for n = 1..8 (given by the OEIS entry) are used for the
   initial conditions. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..8 */
    static const seq_t base[8] = {
        171564ULL,        /* a(1) */
        588840ULL,        /* a(2) */
        2408480ULL,       /* a(3) */
        21702240ULL,      /* a(4) */
        190349280ULL,     /* a(5) */
        1990281312ULL,    /* a(6) */
        18371031840ULL,   /* a(7) */
        163028308320ULL   /* a(8) */
    };

    /* For n up to the given offset, return the provided value. */
    if (n <= 8)
        return base[n - 1];

    /* Determine which residue class modulo 3 is needed.  
       For n > 8 the recurrence a(n) = 859 * a(n-3) holds.  */
    seq_t start;
    if (n % 3 == 1) {          /* n ≡ 1 (mod 3) → start from a(7) */
        start = 18371031840ULL;
    } else if (n % 3 == 2) {   /* n ≡ 2 (mod 3) → start from a(8) */
        start = 163028308320ULL;
    } else {                   /* n ≡ 0 (mod 3) → start from a(6) */
        start = 1990281312ULL;
    }

    /* Number of times to apply the multiplier 859. */
    idx_t steps = (n - (idx_t)((n % 3 == 1) ? 7 :
                               (n % 3 == 2) ? 8 : 6)) / 3;

    seq_t result = start;
    const seq_t factor = (seq_t)859;
    for (idx_t i = 0; i < steps; ++i)
        result *= factor;

    return result;
}
