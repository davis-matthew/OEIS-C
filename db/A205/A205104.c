/*
 * A205104  a(n) = (1/n)*A205103(n).
 * https://oeis.org/A205104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*A205103(n), where A205103(n) = k!! - j!! for the
 *            least k, then least j<k, with k!! == j!! (mod n); found by
 *            search
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t A205104_dfact(uint64_t m)
{
    seq_t r = (seq_t)1;
    while (m > 1) {
        r = r * (seq_t)m;
        m -= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t *first = (uint64_t *)calloc((size_t)N, sizeof(uint64_t));
    if (!first) abort();

    /* r[k] = k!! mod N, tracked via two parity chains */
    uint64_t chain[2];
    chain[0] = 1 % N; /* even chain: 0!! */
    chain[1] = 1 % N; /* odd chain: (-1)!! */
    uint64_t kk = 0, jj = 0;
    first[chain[0]] = 0; /* k=0 not used (indices start at 1) */
    for (uint64_t k = 1; k <= N + 1; ++k) {
        uint64_t p = k & 1;
        chain[p] = (chain[p] * (k % N)) % N;
        uint64_t r = chain[p];
        if (first[r] != 0) {
            kk = k;
            jj = first[r];
            break;
        }
        first[r] = k;
    }
    free(first);
    seq_t diff = A205104_dfact(kk) - A205104_dfact(jj);
    return diff / (seq_t)n;
}
