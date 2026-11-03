/*
 * A104858  Partial sums of the little Schroeder numbers (A001003).
 * https://oeis.org/A104858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 187 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* Partial sums of the little Schröder numbers (OEIS A104858)
   Recurrence (n+1)*a_wide(n) = (7*n-2)*a_wide(n-1) - (7*n-5)*a_wide(n-2) + (n-2)*a_wide(n-3)
   Initial values: a_wide(0)=1, a_wide(1)=2, a_wide(2)=5, a_wide(3)=16 */
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_size = 0;   /* number of entries already filled (next free index) */
static idx_t memo_cap  = 0;   /* capacity of the memo array */

seq_t a_wide(idx_t n) {
    /* Ensure memo array can hold up to index n */
    if (n + 1 > memo_cap) {
        idx_t new_cap = memo_cap ? memo_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, memo, new_cap);
        if (!tmp) return (seq_t)0;          /* allocation failed – unlikely in tests */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = memo_size; i <= n; ++i) {
        switch (i) {
            case 0: memo[0] = (seq_t)1; break;
            case 1: memo[1] = (seq_t)2; break;
            case 2: memo[2] = (seq_t)5; break;
            case 3: memo[3] = (seq_t)16; break;
            default: {
                /* a_wide(i) = ((7*i-2)*a_wide(i-1) - (7*i-5)*a_wide(i-2) + (i-2)*a_wide(i-3))/(i+1) */
                seq_t coeff1 = (seq_t)7 * (seq_t)i - (seq_t)2;
                seq_t coeff2 = (seq_t)7 * (seq_t)i - (seq_t)5;
                seq_t coeff3 = (seq_t)i - (seq_t)2;
                seq_t denom  = (seq_t)i + (seq_t)1;
                seq_t term1 = coeff1 * memo[i-1];
                seq_t term3 = coeff3 * memo[i-3];
                /* term1 + term3 >= term2, so subtraction is safe */
                seq_t numerator = term1 + term3;
                numerator -= coeff2 * memo[i-2];
                memo[i] = numerator / denom;
            } break;
        }
    }
    memo_size = n + 1;
    return memo[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
