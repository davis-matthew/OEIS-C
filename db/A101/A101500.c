/*
 * A101500  A Chebyshev transform of the central binomial numbers.
 * https://oeis.org/A101500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(n-k,k)*(-1)^k*C(2(n-2k), n-2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                 = ((4n-2)*a(n-1) + (4n-6)*a(n-3) - (2n-2)*a(n-2) - (n-2)*a(n-4)) / n */
/* Offset: 0 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include "../utils/resize_array.h"

seq_t a(idx_t n)
{
    /* Small memoization array */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* Basic checks and initial values */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)5;
            case 3: return (seq_t)16;
        }
    }

    /* Grow memo array if needed */
    if (memo_size <= n) {
        idx_t new_size = n + 1;
        memo = resize_array(memo, memo_size, new_size);
        for (idx_t i = memo_size; i < new_size; ++i)
            memo[i] = (seq_t)0;
        memo_size = new_size;

        /* Initialise first four elements */
        if (memo_size > 0 && memo[0] == (seq_t)0) {
            memo[0] = (seq_t)1;
            if (memo_size > 1) memo[1] = (seq_t)2;
            if (memo_size > 2) memo[2] = (seq_t)5;
            if (memo_size > 3) memo[3] = (seq_t)16;
        }
    }

    /* Compute missing values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        if (memo[i] != (seq_t)0) continue;      /* already computed */

        seq_t a1 = memo[i-1];
        seq_t a2 = memo[i-2];
        seq_t a3 = memo[i-3];
        seq_t a4 = memo[i-4];
        seq_t seq_n = (seq_t)i;

        /* Use 128‑bit integers when seq_t is 64‑bit or less (avoids overflow) */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 n = (unsigned __int128)i;

            unsigned __int128 c1 = 4 * n - 2;          /* 4n-2 */
            unsigned __int128 c2 = 4 * n - 6;          /* 4n-6 */
            unsigned __int128 sub1 = 2 * n - 2;        /* 2n-2 */
            unsigned __int128 sub2 = n - 2;            /* n-2 */

            unsigned __int128 term1 = c1 * (unsigned __int128)a1;
            unsigned __int128 term2 = c2 * (unsigned __int128)a3;
            unsigned __int128 sum1  = term1 + term2;

            unsigned __int128 term3 = sub1 * (unsigned __int128)a2;
            unsigned __int128 term4 = sub2 * (unsigned __int128)a4;
            unsigned __int128 sum2  = term3 + term4;

            unsigned __int128 numer = sum1 - sum2;     /* should be >= 0 */
            seq_t result = (seq_t)(numer / n);
            memo[i] = result;
        } else {
            /* Large‑integer path: use seq_t multiplication directly */
            seq_t coeff1 = (seq_t)4 * seq_n - (seq_t)2;   /* 4n-2 */
            seq_t coeff2 = (seq_t)4 * seq_n - (seq_t)6;   /* 4n-6 */
            seq_t coeff3 = (seq_t)2 * seq_n - (seq_t)2;   /* 2n-2 */
            seq_t coeff4 = seq_n - (seq_t)2;              /* n-2 */

            seq_t sum1 = coeff1 * a1 + coeff2 * a3;
            seq_t sum2 = coeff3 * a2 + coeff4 * a4;
            seq_t numer = sum1 - sum2;   /* result should be non‑negative */
            seq_t result = numer / seq_n;
            memo[i] = result;
        }
    }
    return memo[n];
}
