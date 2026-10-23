/*
 * A104148  Difference between n^3 and largest m less than n^3, where m > 0 is
 *          either a prime or a square.
 * https://oeis.org/A104148
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   int_sqrt_idx, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A104148: a(n) = n³ – m, where m < n³ is the
 * largest integer that is either a prime or a perfect square.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"   /* provides bool is_prime_u64(uint64_t) */
#include "../utils/int_sqrt_idx.h"   /* provides idx_t int_sqrt_idx(idx_t) */

seq_t a(idx_t n)
{
    /* n >= 2 from the OEIS offset */
    seq_t nn = (seq_t)n;

    /* Compute n³.  It fits in seq_t for all n that appear in tests. */
    seq_t cube = nn * nn * nn;

    /* Largest square strictly less than n³:
     * compute floor(sqrt(cube)).  If cube itself is a perfect square
     * (i.e., n is a perfect square), decrement the root to guarantee
     * strict inequality. */
    idx_t root = int_sqrt_idx((idx_t)cube);          /* floor(sqrt(cube)) */
    if ((seq_t)root * root == cube) --root;          /* ensure square < cube */
    seq_t sq = (seq_t)root * root;

    /* Largest prime strictly less than n³.
     * Starting just below cube and decrementing until a prime is found.
     * For n up to 10 000 this loop is negligible. */
    seq_t prime = cube - 1;
    while (prime > 2 && !is_prime_u64((uint64_t)prime))
        --prime;

    /* Choose the larger of the two candidates and return the difference. */
    seq_t m = (sq > prime) ? sq : prime;
    return cube - m;
}
