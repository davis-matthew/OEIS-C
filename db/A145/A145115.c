/*
 * A145115  Numbers of length n binary words with fewer than 7 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-8) + a(n-9) (n≥9); initial
 *            terms: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32,
 *            a(6)=64, a(7)=128, a(8)=256
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t cap = 0;
    static size_t len = 0; /* number of computed terms (next free index) */

    /* Ensure capacity for n+1 terms */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        memo = tmp;
        cap = new_cap;
    }

    /* Initialise the first nine terms once */
    if (len < 9) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)4;
        memo[3] = (seq_t)8;
        memo[4] = (seq_t)16;
        memo[5] = (seq_t)32;
        memo[6] = (seq_t)64;
        memo[7] = (seq_t)128;
        memo[8] = (seq_t)256;
        len = 9;
    }

    /* Compute terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = memo[i];            /* already known */
        } else {
            val = 3 * memo[i-1] - 2 * memo[i-2] - memo[i-8] + memo[i-9];
        }
        memo[i] = val;
    }
    len = (size_t)n + 1;

    return memo[n];
}
