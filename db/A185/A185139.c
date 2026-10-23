/*
 * A185139  Triangle T(n,k) = Sum_{i=1..n} 2^(i-1)*C(n+2*k-i-1, k-1), 1 <= k
 *          <= n.
 * https://oeis.org/A185139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=1..r} 2^{i-1}·C(r+2k-i-1, k-1), with shift
 *            determined by the row r and column k of the n‑th term in the
 *            triangle A185139 (offset 1). The first element of row r is 2^r−1
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 77, u64 n <= 274, u128 n <= 989,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* index type provided by the build; assumed to be unsigned 64‑bit */
typedef uint64_t idx_t;

/* triangular numbers: T(r) = r*(r+1)/2, computed via 128‑bit arithmetic */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* multiplicative binomial coefficient: C(n,k)   (works for any seq_t
   supporting + - * and / with exact division) */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; a(1) is first term */

    /* locate row r such that T(r-1) < n ≤ T(r) */
    idx_t r = 1;
    while (triangular(r) < n) ++r;

    idx_t prev = triangular(r - 1);
    idx_t k = n - prev;                  /* column (1‑based) within the row */

    seq_t sum = 0;
    seq_t pow = 1;                       /* 2^0 */

    for (idx_t i = 1; i <= r; ++i) {
        idx_t top = r + 2 * k - i - 1;    /* upper index in binomial */
        seq_t bin = binomial(top, k - 1);/* C(top, k-1) */
        sum += bin * pow;                /* accumulate contribution */
        pow *= 2;                        /* next power of two */
    }
    return sum;
}
