/*
 * A191819  (A178477(n)-1)/9.
 * https://oeis.org/A191819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, base
 * Formula:   a(n) = (p(n) – 1) / 9 where p(n) is the number formed by the
 *            n‑th lexicographic permutation of the digits 1,2,3,4,5,6,7 (p(1)
 *            = 1234567)
 * Range:     u8 none, u16 none, u32 n <= 5040, u64 n <= 5040, u128 n <= 5040,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* There are 7! = 5040 permutations.  For indices larger than that
       we wrap around, which reproduces the sequence periodically.          */
    const idx_t PERM_COUNT = 5040;
    idx_t idx = (n - 1) % PERM_COUNT;          /* zero‑based index       */

    /* Factorial values for 6 down to 0.                                 */
    const idx_t fact[7] = {1ULL,1ULL,2ULL,6ULL,24ULL,120ULL,720ULL};

    /* Current list of digits available for the permutation.            */
    uint8_t digits[7] = {1,2,3,4,5,6,7};

    seq_t value = 0;                           /* result as decimal digits */

    for (idx_t i = 0; i < 7; ++i) {
        idx_t rem = 6 - i;                     /* remaining positions    */
        idx_t f   = fact[rem];
        idx_t pos = idx / f;                   /* choose index from list */
        idx    %= f;                           /* update remainder        */

        value = value * 10 + (seq_t)digits[pos]; /* append digit          */

        /* Remove used digit by shifting the remaining ones left.      */
        for (idx_t j = pos + 1; j < 7 - i; ++j)
            digits[j - 1] = digits[j];
    }

    /* Every such permutation is congruent to 1 (mod 9), so (value-1) is
       divisible by 9 exactly.                                            */
    return (value - 1) / 9;
}
