/*
 * A026519  Irregular triangular array T read by rows: T(n, k) = T(n-1, k-2) +
 *          T(n-1, k) if (n mod 2) = 0, otherwise T(n-1, k-2) + T(n-1, k-1) +
 *          T(n-1, k), with T(n, 0) = T(n, 2*n) = 1, T(n, 1) = T(n, 2*n-1) =
 *          floor((n+1)/2).
 * https://oeis.org/A026519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) read by rows from the irregular triangular array T(n,k)
 *            defined by: T(0,0) = 1. For n ≥ 1: T(n,0) = T(n,2n) = 1, T(n,1)
 *            = T(n,2n-1) = floor((n+1)/2). For n even : T(n,k) = T(n-1,k-2) +
 *            T(n-1,k). For n odd : T(n,k) = T(n-1,k-2) + T(n-1,k-1) +
 *            T(n-1,k). The sequence a(n) is obtained by reading...
 * Range:     u8 n <= 86, u16 n <= 236, u32 n <= 807, u64 n <= 2855,
 *            u128 n <= 9777, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root: floor(sqrt(x)) for 64‑bit unsigned indices. */
static inline idx_t isqrt_idx(idx_t x) {
    /* Uses an algorithm that does not overflow and works for 64‑bit x. */
    idx_t op = x;
    idx_t res = 0;
    idx_t one = (idx_t)1 << 62;          /* highest power of 4 ≤ 2^64-1 */
    while (one != 0) {
        if (op >= res + one) {
            op -= res + one;
            res = (res >> 1) + one;
        } else {
            res >>= 1;
        }
        one >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Locate row r and column k of the index n. */
    idx_t r = isqrt_idx(n);
    idx_t k = n - r * r;

    /* Special case: the very first element. */
    if (r == 0) return (seq_t)1;

    /* Allocate two rows that are large enough for all rows up to r. */
    size_t maxlen = (size_t)(2 * r + 1);
    seq_t *prev = (seq_t *)calloc(maxlen, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(maxlen, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;          /* allocation failure; unlikely */
    }

    /* Row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        size_t len = (size_t)(2 * i + 1);

        /* Boundary values. */
        curr[0] = (seq_t)1;
        curr[len - 1] = (seq_t)1;

        /* Values at k = 1 and k = 2*i-1. */
        seq_t mid = (seq_t)((i + 1) / 2);   /* floor((i+1)/2) */
        curr[1] = mid;
        curr[len - 2] = mid;

        /* Interior values. */
        if (i > 1) {
            for (size_t j = 2; j + 1 < len; ++j) {
                if ((i & 1ULL) == 0) {          /* i even */
                    curr[j] = prev[j - 2] + prev[j];
                } else {                        /* i odd */
                    curr[j] = prev[j - 2] + prev[j - 1] + prev[j];
                }
            }
        }

        /* Prepare for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Retrieve the desired element. */
    seq_t result = prev[(size_t)k];

    /* Clean up. */
    free(prev);
    free(curr);
    return result;
}
