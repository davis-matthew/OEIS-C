/*
 * A194358  Triangle of divisors of 30^n, each number occurring once.
 * https://oeis.org/A194358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n‑th element of the triangle of divisors of 30^k,
 *            where row k contains all divisors of 30^k that are not divisors
 *            of 30^(k‑1), sorted in ascending order. Row k has length
 *            3k^2+3k+1
 * Range:     u8 n <= 23, u16 n <= 114, u32 n <= 506, u64 n <= 3358,
 *            u128 n <= 9260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Row length for row k */
static inline idx_t row_len(idx_t k)
{
    return 3 * k * k + 3 * k + 1;
}

/* Comparison function for unsigned __int128 */
static int cmp_uint128(const void *p, const void *q)
{
    unsigned __int128 a = *(const unsigned __int128 *)p;
    unsigned __int128 b = *(const unsigned __int128 *)q;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Find the row r that contains the n-th term */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + row_len(r) <= n) {
        sum += row_len(r);
        ++r;
    }
    size_t row_offset = (size_t)(n - sum);          /* zero‑based index within the row */
    size_t R = (size_t)r;

    /* 2. Precompute powers of 2, 3, 5 up to exponent R (fits in 128 bits) */
    unsigned __int128 pow2[64] = {0};
    unsigned __int128 pow3[64] = {0};
    unsigned __int128 pow5[64] = {0};

    pow2[0] = pow3[0] = pow5[0] = (unsigned __int128)1;
    for (size_t i = 1; i <= R; ++i) {
        pow2[i] = pow2[i - 1] * (unsigned __int128)2;
        pow3[i] = pow3[i - 1] * (unsigned __int128)3;
        pow5[i] = pow5[i - 1] * (unsigned __int128)5;
    }

    /* 3. Generate all triples (a,b,c) with 0≤a,b,c≤R and at least one = R */
    idx_t row_size = row_len(r);
    unsigned __int128 *row_vals = (unsigned __int128 *)malloc(row_size * sizeof(unsigned __int128));
    if (!row_vals) return (seq_t)0; /* fallback, should not happen */

    size_t len = 0;
    for (size_t a = 0; a <= R; ++a) {
        for (size_t b = 0; b <= R; ++b) {
            for (size_t c = 0; c <= R; ++c) {
                if (a == R || b == R || c == R) {
                    row_vals[len++] = pow2[a] * pow3[b] * pow5[c];
                }
            }
        }
    }

    /* 4. Sort the values */
    qsort(row_vals, len, sizeof(unsigned __int128), cmp_uint128);

    /* 5. Extract the desired term and convert to seq_t */
    seq_t result = (seq_t)row_vals[row_offset];

    /* 6. Free temporary allocation */
    free(row_vals);

    return result;
}
