/*
 * A081921  Expansion of exp(3x)/sqrt(1-x^2).
 * https://oeis.org/A081921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) - 3*a(n-1) -(n-1)^2*a(n-2) +3*(n-1)*(n-2)*a(n-3)=0, with
 *            a(0)=1, a(1)=3, a(2)=10. (OEIS A081921)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t memo_size = 0;   /* allocated size */
    static idx_t memo_last = 0;     /* last index that has been computed */

    /* Initialisation with base values */
    if (!memo) {
        memo_size = 3;
        memo = seq_alloc_new_array(memo_size);
        if (!memo) return 0;           /* out of memory – unlikely in tests */
        memo[0] = 1;
        memo[1] = 3;
        memo[2] = 10;
        memo_last = 2;
    }

    /* Grow the array if necessary */
    if ((size_t)n >= memo_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_arr = seq_alloc_new_array(new_size);
        if (!new_arr) return 0; /* out of memory */
        memcpy(new_arr, memo, memo_size * sizeof(seq_t));
        seq_free_array(memo);
        memo = new_arr;
        memo_size = new_size;
    }

    /* Compute missing entries using the recurrence */
    if (n > memo_last) {
        idx_t start = memo_last + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t term1 = 3 * memo[i - 1];
            seq_t n1 = (seq_t)(i - 1);
            seq_t n2 = (seq_t)(i - 2);
            seq_t term2 = n1 * n1 * memo[i - 2];
            seq_t term3 = 3 * n1 * n2 * memo[i - 3];
            seq_t tmp = term1 + term2;
            memo[i] = tmp - term3;
        }
        memo_last = n;
    }

    return memo[n];
}
