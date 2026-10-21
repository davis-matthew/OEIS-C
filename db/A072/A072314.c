/*
 * A072314  Smallest Fibonacci number containing exactly n 9's.
 * https://oeis.org/A072314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest Fibonacci number containing exactly n 9's
   by iteratively generating Fibonacci numbers and counting the
   number of digit '9' in each.  Results for n up to 20000 are
   memoized.  For larger n the function still works, but does not
   store the value. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

#define MAX_N 20000   /* maximum n for which results are memoized */

static seq_t  ans[MAX_N + 1] = {0}; /* ans[0] unused, ans[1..] store first occurrence */
static bool   initialized = false;
static idx_t  fib_index   = 2;          /* index of fib_curr  (F2)          */
static seq_t  fib_prev   = 1;          /* F1 */
static seq_t  fib_curr   = 1;          /* F2 */

static void init(void) {
    if (initialized) return;
    initialized = true;
    /* ans array is zero-initialized by the compiler, which serves
       as the sentinel value (no Fibonacci number equals zero). */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not defined for n = 0 */
    init();

    /* Handle small n with memoization */
    if (n <= MAX_N) {
        if (ans[n] != 0) return ans[n];

        unsigned char cnt[10];
        while (ans[n] == 0) {
            seq_t next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next;
            ++fib_index;

            digit_counts(next, cnt);
            unsigned int c9 = cnt[9];
            if (c9 <= MAX_N) {
                if (ans[c9] == 0) ans[c9] = next;
            }
        }
        return ans[n];
    }

    /* Handle n > MAX_N: compute on the fly without storing */
    unsigned char cnt[10];
    while (true) {
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
        ++fib_index;

        digit_counts(next, cnt);
        if (cnt[9] == n) return next;
    }
}
