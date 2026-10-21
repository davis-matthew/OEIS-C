/*
 * A207106  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 24*n^2 - 10*n)/3 (equivalent to a(n) = 4*a(n‑1)
 *            - 6*a(n‑2) + 4*a(n‑3) - a(n‑4) for n>4, with a(1)=6, a(2)=36,
 *            a(3)=98, a(4)=200)
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 64‑bit intermediate to avoid overflow for 32‑bit seq_t */
    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;          /* n^2 */
    uint64_t n3 = n2 * nn;          /* n^3 */
    uint64_t num = 4ull * n3
                 + 24ull * n2
                 - 10ull * nn;      /* (4*n^3 + 24*n^2 - 10*n) */
    uint64_t res64 = num / 3ull;    /* guaranteed divisible by 3 */
    return (seq_t)res64;
}
