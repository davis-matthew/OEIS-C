/*
 * A098308  Unsigned member r=-8 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A098308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) + 2*(-1)^(n+1) Initial values: a(0)=0,
 *            a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;          /* number of elements stored (max index+1) */

    if (n + 1 > memo_size) {
        /* Ensure we have space up to index n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;       /* allocation failed – fallback */
        memo = tmp;

        /* Compute any missing values from old size up to n */
        for (idx_t i = memo_size; i <= n; ++i) {
            if (i == 0)
                memo[i] = (seq_t)0;
            else if (i == 1)
                memo[i] = (seq_t)1;
            else {
                seq_t temp = memo[i-1] * ((seq_t)10) - memo[i-2];
                if ((i & 1) == 0)          /* even n: (-1)^(n+1) = -1 */
                    temp = temp - ((seq_t)2);
                else                        /* odd n:  (-1)^(n+1) =  1 */
                    temp = temp + ((seq_t)2);
                memo[i] = temp;
            }
        }
        memo_size = new_size;
    }
    return memo[n];
}
