/*
 * A205786  Least positive integer j such that n divides C(k)-C(j), where k,
 *          as in A205785, is the least number for which there is such a j,
 *          and C=A205825.
 * https://oeis.org/A205786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least j with n | C(k)-C(j), where k = A205785(n) is the
 *            least k having such a j<k, and C(m) = m!/ceiling(m/2)! (A205825,
 *            used with m>=1). Computed by search over residues of C(m) mod n
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t A205786_impl(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t *first = (uint64_t *)calloc((size_t)N, sizeof(uint64_t));
    if (!first) abort();
    seq_t res = 0;
    for (uint64_t k = 1; k <= N + 1; ++k) {
        uint64_t r = 1 % N;
        for (uint64_t i = (k + 1) / 2 + 1; i <= k; ++i)
            r = (uint64_t)(((unsigned __int128)r * i) % N);
        if (first[r]) {
            res = (seq_t)first[r];
            break;
        }
        first[r] = k;
    }
    free(first);
    return res;
}

seq_t a(idx_t n)
{
    return A205786_impl(n);
}
