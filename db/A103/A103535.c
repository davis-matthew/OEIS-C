/*
 * A103535  Number of nets in a regular right prism.
 * https://oeis.org/A103535
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) – 6*a(n-2) – 18*a(n-3) + 38*a(n-4) – 18*a(n-5) –
 *            6*a(n-6) + 6*a(n-7) – a(n-8) Initial values (index : value): 3 :
 *            9 4 : 29 5 : 99 6 : 354 7 : 1290 8 : 4762 9 : 17663 10 : 65733
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build */
static seq_t *memo = NULL;          /* array holding values a(3) .. a(max computed) */
static idx_t  memo_size = 0;        /* number of stored entries */
static const idx_t offset = 3;      /* a(3) is stored at memo[0] */

/* initialize memo with the first 8 values (a(3)..a(10)) */
static void init_memo(void) {
    if (memo != NULL) return;
    memo = (seq_t *)malloc(8 * sizeof(seq_t));
    memo_size = 8;
    memo[0] = 9;
    memo[1] = 29;
    memo[2] = 99;
    memo[3] = 354;
    memo[4] = 1290;
    memo[5] = 4762;
    memo[6] = 17663;
    memo[7] = 65733;
}

seq_t a(idx_t n) {
    if (n < offset) return 0;          /* value for n < 3 is irrelevant */

    init_memo();

    /* grow memo array if needed */
    if (n >= offset + memo_size) {
        idx_t needed = n - offset + 1;        /* total entries required */
        memo = (seq_t *)realloc(memo, needed * sizeof(seq_t));
        /* compute new entries */
        for (idx_t i = memo_size; i < needed; ++i) {
            /* indices in memo correspond to a(3), a(4), ... */
            /* we compute a(i+3) */
            if (i < 8) { /* already initialised */
                continue;
            } else {
                seq_t a1 = memo[i - 1];      /* a(n-1) */
                seq_t a2 = memo[i - 2];      /* a(n-2) */
                seq_t a3 = memo[i - 3];      /* a(n-3) */
                seq_t a4 = memo[i - 4];      /* a(n-4) */
                seq_t a5 = memo[i - 5];      /* a(n-5) */
                seq_t a6 = memo[i - 6];      /* a(n-6) */
                seq_t a7 = memo[i - 7];      /* a(n-7) */
                seq_t a8 = memo[i - 8];      /* a(n-8) */

                /* Positive part: 6*a1 + 38*a4 + 6*a7 */
                seq_t pos = 6 * a1;
                pos = pos + (38 * a4);
                pos = pos + (6 * a7);

                /* Negative part: 6*a2 + 18*a3 + 18*a5 + 6*a6 + a8 */
                seq_t neg = 6 * a2;
                neg = neg + (18 * a3);
                neg = neg + (18 * a5);
                neg = neg + (6 * a6);
                neg = neg + a8;

                memo[i] = pos - neg;
            }
        }
        memo_size = needed;
    }

    return memo[n - offset];
}
