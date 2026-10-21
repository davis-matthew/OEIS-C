/*
 * A080001  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,0,2}.
 * https://oeis.org/A080001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+2*a(n-5)-a(n-9)-a(n-10) for n>=10 with
 *            initial values:
 *            a(0)=1,a(1)=0,a(2)=0,a(3)=1,a(4)=1,a(5)=1,a(6)=1,a(7)=2,a(8)=4,a(9)=3
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[10] = {1,0,0,1,1,1,1,2,4,3};

static seq_t *memo = NULL;
static size_t memo_size = 0;

seq_t a(idx_t n) {
    /* Direct return for small n */
    if (n < 10)
        return init_vals[(size_t)n];

    size_t need = (size_t)n + 1;

    if (memo == NULL) {
        /* First time: allocate up to the required size and compute values */
        memo = (seq_t *)malloc(need * sizeof(seq_t));
        if (!memo) return 0;
        for (size_t i = 0; i < 10; ++i)
            memo[i] = init_vals[i];
        for (size_t i = 10; i < need; ++i)
            memo[i] = memo[i-3] + memo[i-4] + (seq_t)2 * memo[i-5]
                     - memo[i-9] - memo[i-10];
        memo_size = need;
    } else if (memo_size < need) {
        /* Grow the allocation and compute new entries */
        seq_t *tmp = (seq_t *)realloc(memo, need * sizeof(seq_t));
        if (!tmp) return 0;
        memo = tmp;
        for (size_t i = memo_size; i < need; ++i)
            memo[i] = memo[i-3] + memo[i-4] + (seq_t)2 * memo[i-5]
                     - memo[i-9] - memo[i-10];
        memo_size = need;
    }

    return memo[(size_t)n];
}
