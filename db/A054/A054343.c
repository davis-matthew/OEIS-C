/*
 * A054343  Number of nonnegative integer 3 X 3 matrices with sum of elements
 *          equal to n, under action of dihedral group of the square D_4.
 * https://oeis.org/A054343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 5*a(n-1) - 8*a(n-2) +
 *            16*a(n-4) - 24*a( n-5) + 16*a(n-6) + 8*a(n-7) - 34*a(n-8) +
 *            34*a(n-9) - 8*a(n-10) - 16*a(n-11) + 24*a(n-12) - 16*a(n-13) +
 *            8*a(n-15) - 5*a(n-16) + a(n-17) (n>16) Initial values: a(0)=1,
 *            a(1)=3, a(2)=11, a(3)=31, a(4)=84, a(5)=198, a(...
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values up to n=17 */
    static const seq_t init[18] = {
        1ULL, 3ULL, 11ULL, 31ULL, 84ULL, 198ULL, 440ULL, 904ULL,
        1766ULL, 3266ULL, 5802ULL, 9906ULL, 16384ULL, 26284ULL,
        41104ULL, 62752ULL, 93831ULL, 137589ULL
    };
    static const int coeff[17] = {
        5,   -8,   0,   16,  -24,  16,   8,
       -34,  34,  -8, -16,   24, -16,   0,
        8,   -5,   1
    };
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    static size_t memo_cap = 0;

    /* Return immediately for small indices */
    if (n < 18)
        return init[n];

    /* Initialise memoisation array on first use */
    if (memo_len == 0) {
        memo_cap = 64;
        if (memo_cap < 18) memo_cap = 18;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i)
            memo[i] = init[i];
        memo_len = 18;
    }

    /* Grow the memo array to reach n */
    if (n >= memo_len) {
        if ((size_t)(n + 1) > memo_cap) {
            size_t new_cap = memo_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            memo_cap = new_cap;
        }

        for (idx_t i = memo_len; i <= n; ++i) {
            seq_t positives = 0;
            seq_t negatives = 0;
            for (int lag = 1; lag <= 17; ++lag) {
                int c = coeff[lag - 1];
                if (c == 0 || i < (idx_t)lag) continue;
                seq_t term = memo[i - lag];
                if (c > 0)
                    positives = positives + term * (seq_t)c;
                else
                    negatives = negatives + term * (seq_t)(-c);
            }
            memo[i] = positives - negatives;
        }
        memo_len = n + 1;
    }

    return memo[n];
}
