/*
 * A240019  Number of partitions of n, where the difference between the number
 *          of odd parts and the number of even parts is 10.
 * https://oeis.org/A240019
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0, 3k+10≤n} coeff_{x^{n-(3k+10)}} ∏_{j=1}^{k+10}
 *            (1–x^{2j})^{-c_j}, where c_j = 2 for j≤k, c_j = 1 otherwise. The
 *            DP “ways[s]” stores the coefficient of x^s in that product and
 *            is updated incrementally when k increases by one: multiply by
 *            (1–x^{2(k+1)})^{-1} (adds a second copy of par...
 * Range:     u8 n <= 33, u16 n <= 65, u32 n <= 162, u64 n <= 482,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 10) return 0;        /* values below the offset are irrelevant */

    idx_t N = (idx_t)n;
    seq_t *ways = (seq_t *)calloc((size_t)N + 1, sizeof(seq_t));
    if (!ways) return 0;         /* memory allocation failed – should not happen */

    ways[0] = 1;

    /* initialise for k = 0: parts 2,4,…,20 each with exponent 1 */
    for (idx_t j = 1; j <= 10; ++j) {
        idx_t part = 2 * j;
        for (idx_t s = part; s <= N; ++s) {
            ways[s] += ways[s - part];
        }
    }

    seq_t result = 0;
    idx_t k = 0;
    idx_t shift = 10;             /* shift = 3k + 10 for current k */

    while (shift <= n) {
        result += ways[n - shift];

        /* prepare for the next k */
        idx_t next_k = k + 1;
        idx_t next_shift = 3 * next_k + 10;
        if (next_shift > n) break;   /* no further contributions needed */

        /* add an extra copy of part 2*next_k (c_j increases from 1 to 2) */
        idx_t part_copy = 2 * next_k;
        if (part_copy <= N) {
            for (idx_t s = part_copy; s <= N; ++s) {
                ways[s] += ways[s - part_copy];
            }
        }

        /* add a new part 2*(next_k+10) with exponent 1 */
        idx_t part_new = 2 * (next_k + 10);   /* (next_k+10)*2 */
        if (part_new <= N) {
            for (idx_t s = part_new; s <= N; ++s) {
                ways[s] += ways[s - part_new];
            }
        }

        k = next_k;
        shift = next_shift;
    }

    free(ways);
    return result;
}
