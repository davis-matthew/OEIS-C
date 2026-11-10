/*
 * A059234  The array in A059216 read by antidiagonals in the direction in
 *          which it was constructed.
 * https://oeis.org/A059234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   antidiagonals of the array of A059216 in construction order.
 *            Diagonal s=i+j (s>=2) has cells A(i,j); the first cell is 1. s
 *            even: i goes 1..s-1 downward, A(i,j) = previous cell + sum of
 *            column j so far. s odd: i goes s-1..1 upward, A(i,j) = previous
 *            cell + sum of row i so far. a(n) = n-th cell in...
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* find the diagonal s containing term n (diagonal s has s-1 terms) */
    idx_t rem = n, S = 2;
    while (rem > S - 1) { rem -= S - 1; S++; }

    seq_t *rowSum = mem_new(seq_t, (size_t)S + 2);
    seq_t *colSum = mem_new(seq_t, (size_t)S + 2);
    seq_t result = 0;

    for (idx_t t = 2; t <= S; ++t) {
        seq_t prev = 0;
        idx_t cnt = 0;
        for (idx_t k = 1; k <= t - 1; ++k) {
            idx_t i = (t % 2 == 0) ? k : (t - k);
            idx_t j = t - i;
            seq_t val;
            if (k == 1) val = 1;
            else if (t % 2 == 0) val = prev + colSum[j];
            else val = prev + rowSum[i];
            prev = val;
            rowSum[i] += val;
            colSum[j] += val;
            cnt = k;
            if (t == S && cnt == rem) { result = val; }
        }
    }
    mem_free(rowSum);
    mem_free(colSum);
    return result;
}
