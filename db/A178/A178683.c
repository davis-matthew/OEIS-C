/*
 * A178683  Shortest partition of n with maximal product, sorted descending &
 *          considered as a base-5 number.
 * https://oeis.org/A178683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A178683: shortest partition of n with maximal product,
   sorted descending and interpreted as a base‑5 number. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;

    /* For n >= 5 use the well‑known optimal partition:
       all 3's, possibly a 4 (if remainder 1) or a 2 (if remainder 2),
       which also gives the minimal number of parts. */
    idx_t rem = n % 3;
    seq_t val = 0;

    if (rem == 0) {                     /* all 3's */
        idx_t k = n / 3;
        for (idx_t i = 0; i < k; ++i)
            val = val * 5 + 3;
    } else if (rem == 1) {               /* one 4, rest 3's */
        val = 4;
        idx_t k = (n - 4) / 3;
        for (idx_t i = 0; i < k; ++i)
            val = val * 5 + 3;
    } else { /* rem == 2 */               /* one 2, rest 3's */
        idx_t k = (n - 2) / 3;
        for (idx_t i = 0; i < k; ++i)
            val = val * 5 + 3;
        val = val * 5 + 2;
    }

    return val;
}
