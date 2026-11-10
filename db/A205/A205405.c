/*
 * A205405  s(A205403), where s(j)=floor[(j+1)^2/2].
 * https://oeis.org/A205405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(A205403(n)) with s(j) = floor((j+1)^2/4); A205403(n) =
 *            j, the earlier index of the first repeated residue of s(.) mod n
 *            (search by first-occurrence table)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) {
        abort();
    }

    for (idx_t k = 1; ; ++k) {
        seq_t kp1 = (seq_t)k + 1;
        seq_t s = (kp1 * kp1) / 4;
        idx_t r = (idx_t)(s % (seq_t)n);

        if (first[r] == 0) {
            first[r] = k;
        } else {
            idx_t j = first[r];
            free(first);
            seq_t jp1 = (seq_t)j + 1;
            return (jp1 * jp1) / 4;
        }
    }
}
