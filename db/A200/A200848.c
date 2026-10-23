/*
 * A200848  Primes of the form 3n^3-4.
 * https://oeis.org/A200848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the primes of the form 3*n³ − 4. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, because offset is 1 */

    idx_t    x   = 0;   /* current n in 3*n³-4 */
    idx_t    cnt = 0;   /* how many primes found so far */
    seq_t    val = 0;   /* candidate prime */

    while (cnt < n) {
        x++;
        /* compute 3*x³ – 4 using seq_t to avoid overflow */
        seq_t tx   = (seq_t)x;
        seq_t cube = tx * tx * tx;
        val = (seq_t)3 * cube - (seq_t)4;

        if (is_prime_seq(val))
            cnt++;
    }
    return val;
}
