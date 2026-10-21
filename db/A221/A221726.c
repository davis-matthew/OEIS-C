/*
 * A221726  Number of nX3 arrays of occupancy after each element stays put or
 *          moves to some horizontal, diagonal or antidiagonal neighbor, with
 *          every occupancy equal to zero or two.
 * https://oeis.org/A221726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 44*a(n-2) - 462*a(n-4) + 1400*a(n-6)
 *            - 1417*a(n-8) - 448*a(n-10) - 280*a(n-12) + 100*a(n-14) a(1) =
 *            0, a(2) = 16, a(3) = 0, a(4) = 464, a(5) = 0, a(6) = 13948, a(7)
 *            = 0, a(8) = 420736, a(9) = 0, a(10) = 12695288, a(11) = 0, a(12)
 *            = 383074904, a(13) = 0, a(14) = 1155912...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *memo = NULL;        /* memoized sequence values (1‑based) */
static size_t memo_cap = 0;       /* highest index that is stored */

seq_t a(idx_t n)
{
    /* Ensure the memo array is allocated and large enough */
    if (memo_cap == 0) {
        /* Allocate up to index 14, which are the initial values */
        memo_cap = 14;
        memo = (seq_t *)malloc(sizeof(seq_t) * (memo_cap + 1)); /* index 0 unused */
        if (!memo) return 0;               /* out of memory – return 0 */
        /* initialise base terms */
        memo[0] = 0;
        memo[1] = 0;
        memo[2] = 16;
        memo[3] = 0;
        memo[4] = 464;
        memo[5] = 0;
        memo[6] = 13948;
        memo[7] = 0;
        memo[8] = 420736;
        memo[9] = 0;
        memo[10] = 12695288;
        memo[11] = 0;
        memo[12] = 383074904;
        memo[13] = 0;
        memo[14] = 11559126552;
    }

    /* If the requested term is within the cached range, return it */
    if (n <= memo_cap) return memo[n];

    /* Expand the cache to accommodate at least n */
    size_t new_cap = n;
    if (new_cap < memo_cap + 64) new_cap = memo_cap + 64; /* grow in chunks */
    seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * (new_cap + 1));
    if (!tmp) return 0;               /* allocation failure */
    memo = tmp;

    /* Compute missing terms sequentially */
    for (idx_t i = memo_cap + 1; i <= n; ++i) {
        if (i & 1) {                 /* odd indices are zero */
            memo[i] = 0;
        } else if (i <= 14) {        /* already initialised */
            continue;
        } else {                     /* recurrence for even i ≥ 16 */
            seq_t pos = 44 * memo[i - 2] + 1400 * memo[i - 6] + 100 * memo[i - 14];
            seq_t neg = 462 * memo[i - 4] + 1417 * memo[i - 8] +
                       448 * memo[i - 10] + 280 * memo[i - 12];
            memo[i] = pos - neg;    /* pos dominates, so no unsigned underflow */
        }
    }
    memo_cap = n;                      /* update cache size */
    return memo[n];
}
