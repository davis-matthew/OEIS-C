/*
 * A221656  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, without
 *          move-in move-out left turns.
 * https://oeis.org/A221656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence for A221656:
   a(1) = 0, a(2) = 4, a(3) = 24, a(4) = 165, a(5) = 1054,
   a(6) = 6862, a(7) = 44576, a(8) = 289789, a(9) = 1883406,
   a(10) = 12241008, a(11) = 79558150, a(12) = 517073937.
   For n > 12:
   a(n) = 9*a(n-1) -15*a(n-2) -15*a(n-3) +52*a(n-4)
          -51*a(n-5) -a(n-6) +127*a(n-7) -32*a(n-8)
          -28*a(n-9) +25*a(n-10).              */

#include <stdlib.h>

static const seq_t init_terms[13] = {
    0ULL,          /* index 0 (unused) */
    0ULL,          /* a(1) */
    4ULL,          /* a(2) */
    24ULL,         /* a(3) */
    165ULL,        /* a(4) */
    1054ULL,       /* a(5) */
    6862ULL,       /* a(6) */
    44576ULL,      /* a(7) */
    289789ULL,     /* a(8) */
    1883406ULL,    /* a(9) */
    12241008ULL,   /* a(10) */
    79558150ULL,   /* a(11) */
    517073937ULL   /* a(12) */
};

seq_t a(idx_t n)
{
    /* static cache: memo[i] holds a(i), 1‑indexed */
    static seq_t *memo = NULL;
    static size_t memo_size = 0;
    static idx_t computed = 0;          /* highest index computed */

    /* initialise cache on first call or when array too small */
    if (memo == NULL) {
        size_t needed = n > 13 ? (size_t)n + 1 : 14;
        memo_size = needed;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        for (idx_t i = 1; i <= 12 && i < memo_size; ++i)
            memo[i] = init_terms[i];
        for (idx_t i = 13; i < memo_size; ++i)
            memo[i] = 0;
        computed = 12;
    } else if (n + 1 > memo_size) {
        size_t new_size = memo_size * 2;
        if (new_size < (size_t)n + 1) new_size = (size_t)n + 1;
        memo = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        memo_size = new_size;
    }

    /* compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 12) {
            memo[i] = init_terms[i];
            continue;
        }
        seq_t pos = 0, neg = 0;
        pos += 9   * memo[i-1];
        pos += 52  * memo[i-4];
        pos += 127 * memo[i-7];
        pos += 25  * memo[i-10];
        neg += 15  * memo[i-2];
        neg += 15  * memo[i-3];
        neg += 51  * memo[i-5];
        neg +=     memo[i-6];
        neg += 32  * memo[i-8];
        neg += 28  * memo[i-9];
        memo[i] = pos - neg;   /* pos ≥ neg for all valid n */
    }

    computed = n;
    return memo[n];
}
