/*
 * A198247  Number of 2n X 4 0..4 arrays with values 0..4 introduced in row
 *          major order and each element equal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 20*a(n-1) -85*a(n-2) -16*a(n-3) +493*a(n-4)
                 -124*a(n-5) -919*a(n-6) +120*a(n-7) +512*a(n-8)
   Initial values: a(1)=1,a(2)=5,a(3)=40,a(4)=485,a(5)=6528,a(6)=90641,a(7)=1268648,a(8)=17794141 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Memoization table */
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t memo_size = 0;

    /* Initial values for n = 1 .. 8 */
    static const seq_t init[8] = {
        (seq_t)1ULL,
        (seq_t)5ULL,
        (seq_t)40ULL,
        (seq_t)485ULL,
        (seq_t)6528ULL,
        (seq_t)90641ULL,
        (seq_t)1268648ULL,
        (seq_t)17794141ULL
    };

    if (n == 0) { /* undefined, return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Ensure capacity for the requested index */
    if (n > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_memo = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_memo) return (seq_t)0;          /* allocation failed */
        if (memo) {
            memcpy(new_memo, memo, memo_size * sizeof(seq_t));
            free(memo);
        }
        memo = new_memo;
        memo_cap = (idx_t)new_cap;
    }

    /* Fill initial terms if not yet done */
    if (memo_size < 8 && n > memo_size) {
        idx_t up = 8;
        if (n < up) up = n;
        for (idx_t i = memo_size; i < up; ++i) {
            memo[i] = init[i];
        }
        if (memo_size < up) {
            /* nothing else to do until we compute the rest */
        }
    }

    /* Compute missing terms using recurrence */
    for (idx_t i = memo_size; i < n; ++i) {
        if (i < 8) continue; /* already initialized */
        seq_t t0 = 20 * memo[i - 1];
        seq_t t1 = 85 * memo[i - 2];
        seq_t t2 = 16 * memo[i - 3];
        seq_t t3 = 493 * memo[i - 4];
        seq_t t4 = 124 * memo[i - 5];
        seq_t t5 = 919 * memo[i - 6];
        seq_t t6 = 120 * memo[i - 7];
        seq_t t7 = 512 * memo[i - 8];
        seq_t res = t0;
        res -= t1;
        res -= t2;
        res += t3;
        res -= t4;
        res -= t5;
        res += t6;
        res += t7;
        memo[i] = res;
    }
    memo_size = n;

    return memo[n - 1];
}
