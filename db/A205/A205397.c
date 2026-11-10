/*
 * A205397  s(A205395), where s(j)=ceiling(j^2/2).
 * https://oeis.org/A205397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(j^2/2) where j = A205395(n); j found by search
 *            over residues of ceiling(i^2/2) mod n
 * Range:     u8 n <= 57, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t *first = (uint64_t *)calloc((size_t)n, sizeof(uint64_t));
    if (first == NULL)
        return (seq_t)0;

    uint64_t j = 0;
    for (uint64_t i = 1;; ++i) {
        uint64_t si = (i * i + 1) / 2;
        uint64_t r = si % (uint64_t)n;
        if (first[r] == 0) {
            first[r] = i;
        } else {
            j = first[r];
            break;
        }
    }
    free(first);

    return ((seq_t)j * (seq_t)j + 1) / 2;
}
