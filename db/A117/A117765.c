/*
 * A117765  Lucas numbers for which the sum of the digits is a Fibonacci
 *          number.
 * https://oeis.org/A117765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   digit_sum, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: enumerate Lucas numbers, compute digit sum, test whether
 * the sum is a Fibonacci number, and cache the qualifying Lucas numbers
 * until the requested index n is reached. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_sum.h"

#define MAX_SUM 20000  /* maximum possible digit-sum for 4‑digit Lucas numbers within n≈10000 */

seq_t a(idx_t n)
{
    /* Static state for caching */
    static bool initialized = false;
    static size_t good_count = 0;       /* number of qualifying Lucas numbers found */
    static size_t good_cap   = 0;       /* capacity of the cached array */
    static seq_t *good = NULL;         /* cached qualifying Lucas numbers */

    static seq_t L_prev = 0;           /* L(k-1) */
    static seq_t L_curr = 0;           /* L(k) */
    static idx_t  cur_idx = 0;         /* current Lucas index (L1=1, L2=3, ...) */

    static bool fib_ok[MAX_SUM + 1] = {false};

    /* One‑time initialization */
    if (!initialized) {
        /* Fibonacci lookup table */
        seq_t f0 = 0, f1 = 1;
        while (f0 <= (seq_t)MAX_SUM) {
            fib_ok[(int)f0] = true;
            seq_t tmp = f0 + f1;
            f0 = f1;
            f1 = tmp;
        }

        /* Cached array init */
        good_cap = 16;
        good = seq_alloc(good_cap);
        good_count = 0;

        /* Start with Lucas numbers L0=2 (ignored), L1=1 */
        L_prev = 2;
        L_curr = 1;
        cur_idx = 1;

        initialized = true;
    }

    /* Extend cache until we have at least n qualifying numbers */
    while (good_count < n) {
        /* Check if current Lucas number qualifies */
        seq_t digit_sum_val = digit_sum(L_curr);
        idx_t sum = (idx_t)digit_sum_val;   /* safe: sum ≤ MAX_SUM */

        if (sum <= MAX_SUM && fib_ok[(int)sum]) {
            /* Ensure cache capacity */
            if (good_count == good_cap) {
                size_t new_cap = good_cap * 2;
                seq_t *tmp = seq_realloc(good, good_cap, new_cap);
                if (tmp == NULL) { /* allocation failure – abort */
                    return 0;
                }
                good = tmp;
                good_cap = new_cap;
            }
            good[good_count++] = L_curr;
        }

        /* Generate next Lucas number */
        seq_t next = L_prev + L_curr;
        L_prev = L_curr;
        L_curr = next;
        cur_idx++;
    }

    return good[n - 1];
}
