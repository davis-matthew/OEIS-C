/*
 * A186320  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and heptagonal
 *          numbers. Complement of A186321.
 * https://oeis.org/A186320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Hcount, where Hcount is the number of heptagonal
 *            numbers strictly less than n². Hcount is obtained as j =
 *            floor((3 + floor(sqrt(9 + 40·n²))) / 10) Hj = j(5j – 3) / 2
 *            Hcount = j – 1 if Hj == n², else j
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid overflow. */
    seq_t N = (seq_t)n * (seq_t)n;                  /* n² */
    seq_t D = (seq_t)9 + (seq_t)40 * N;             /* 9 + 40 n² */
    seq_t sqrtD = floor_sqrt(D);                    /* floor(sqrt(D)) */

    seq_t j = (sqrtD + (seq_t)3) / (seq_t)10;        /* floor((3 + sqrtD) / 10) */
    /* Compute the j‑th heptagonal number Hj = j(5j – 3)/2. */
    seq_t Hj = j * (((seq_t)5 * j) - (seq_t)3) / (seq_t)2;

    seq_t Hcount = (Hj == N) ? j - (seq_t)1 : j;     /* exclude equality */

    return (seq_t)n + Hcount;
}
