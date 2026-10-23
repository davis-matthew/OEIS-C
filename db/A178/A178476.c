/*
 * A178476  Permutations of 123456: Numbers having each of the decimal digits
 *          1,...,6 exactly once, and no other digit.
 * https://oeis.org/A178476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn, base
 * Formula:   a(n) = nth lexicographically smallest 6‑digit permutation of
 *            digits 1..6
 * Range:     u8 none, u16 none, u32 n <= 720, u64 n <= 720, u128 n <= 720,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n‑th term of OEIS A178476.  For n > 720 the result wraps
   around as if the sequence were periodic of period 720. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 6! = 720 permutations */
    const idx_t period = 720;
    if (n == 0) return 0;                /* undefined, return 0 */

    /* Zero‑based rank within the period */
    idx_t r = (n - 1) % period;

    /* Array of available digits */
    int digits[6] = {1, 2, 3, 4, 5, 6};

    /* Precomputed factorials 0! … 5! */
    static const idx_t fact[7] = {1, 1, 2, 6, 24, 120, 720};

    seq_t result = 0;

    /* Build the number digit by digit */
    for (int pos = 0; pos < 6; ++pos) {
        idx_t f = fact[5 - pos];         /* (5-pos)! */
        idx_t di = r / f;                /* index into digits[] */
        r %= f;

        int d = digits[di];              /* chosen digit */

        /* Shift remaining digits left */
        for (int j = di; j < 5 - pos; ++j)
            digits[j] = digits[j + 1];

        result = result * 10 + d;        /* append the chosen digit */
    }

    return result;
}
