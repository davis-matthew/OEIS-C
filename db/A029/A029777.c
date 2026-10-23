/*
 * A029777  Cubes k such that digits of cube root of k appear in k.
 * https://oeis.org/A029777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A029777(n) = A029776(n)^3
   where A029776(n) is the nth integer r for which each decimal digit
   of r occurs at least once in r^3. */
#include <stdbool.h>

/* Return true if all decimal digits that appear in `root` also appear
   in its cube.  Only presence is required, not multiplicity. */
static bool passes(idx_t root)
{
    bool root_digits[10]  = {false};
    bool cube_digits[10]  = {false};

    /* Record digits of the root. */
    idx_t r = root;
    if (r == 0) {
        root_digits[0] = true;
    } else {
        while (r > 0) {
            root_digits[r % 10] = true;
            r /= 10;
        }
    }

    /* Compute the cube. */
    seq_t cube = (seq_t)root * (seq_t)root * (seq_t)root;

    /* Record digits of the cube. */
    seq_t c = cube;
    if (c == 0) {
        cube_digits[0] = true;
    } else {
        while (c > 0) {
            int d = (int)(c % 10);
            cube_digits[d] = true;
            c /= 10;
        }
    }

    /* Check that every digit of the root occurs in the cube. */
    for (int d = 0; d < 10; ++d) {
        if (root_digits[d] && !cube_digits[d]) {
            return false;
        }
    }
    return true;
}

/* Main sequence function.  The OEIS offset is 1, so a(1) is the
   first term listed.  The function works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined for n=0, return 0 */
        return (seq_t)0;
    }

    idx_t found = 0;
    idx_t root  = 0;

    while (true) {
        if (passes(root)) {
            ++found;
            if (found == n) {
                return (seq_t)root * (seq_t)root * (seq_t)root;
            }
        }
        ++root;
    }
}
