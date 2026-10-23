/*
 * A165898  a(n) = sum of the quadratic nonresidues of n.
 * https://oeis.org/A165898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of quadratic nonresidues modulo n
 * Range:     u8 n <= 27, u16 n <= 383, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Allocate a table of residues */
    size_t sz = (size_t)n;
    bool *res = (bool*)malloc(sz);
    if (!res)
        return 0;
    for (size_t i = 0; i < sz; ++i)
        res[i] = false;

    /* Mark all quadratic residues */
    for (size_t x = 1; x < sz; ++x) {
        uintptr_t sq = ((uintptr_t)x * (uintptr_t)x) % n;
        res[sq] = true;
    }

    /* Sum all nonresidues */
    seq_t sum = 0;
    for (size_t k = 1; k < sz; ++k) {
        if (!res[k])
            sum += (seq_t)k;
    }

    free(res);
    return sum;
}
