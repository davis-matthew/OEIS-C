/*
 * A205111  a(n) = A205110(n)/n.
 * https://oeis.org/A205111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A205110(n)/n, where A205110(n) = s(k)-s(j) for the least
 *            k with some j<k, n | s(k)-s(j), s(t)=3^t-2^t; found by searching
 *            for the first repeated residue mod n
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;

    idx_t p3 = 3 % n, p2 = 2 % n;
    idx_t k = 1, j = 0;
    for (;; ++k) {
        idx_t r = (p3 + n - p2) % n;
        if (first[r] != 0) { j = first[r]; break; }
        first[r] = k;
        p3 = (p3 * 3) % n;
        p2 = (p2 * 2) % n;
    }
    free(first);

    seq_t P3k = 1, P2k = 1, P3j = 1, P2j = 1;
    for (idx_t i = 0; i < k; ++i) { P3k *= 3; P2k *= 2; }
    for (idx_t i = 0; i < j; ++i) { P3j *= 3; P2j *= 2; }
    seq_t diff = (P3k - P3j) - (P2k - P2j);
    return diff / (seq_t)n;
}
