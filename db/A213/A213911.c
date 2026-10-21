/*
 * A213911  Number of runs of consecutive zeros in the Zeckendorf (binary)
 *          representation of n.
 * https://oeis.org/A213911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of runs of consecutive zeros in the Zeckendorf
 *            (binary) representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* static table of Fibonacci numbers (F1=1, F2=2) */
    static idx_t *fib = NULL;
    static size_t fib_cnt = 0;  /* last valid index in fib */
    static size_t fib_cap = 0;

    if (fib == NULL) {
        fib_cap = 4;
        fib = (idx_t*)malloc(fib_cap * sizeof(idx_t));
        if (!fib)
            return 0;
        fib[0] = 0;  /* unused sentinel */
        fib[1] = 1;  /* F1 */
        fib[2] = 2;  /* F2 */
        fib_cnt = 2;
    }

    /* grow Fibonacci table until fib[fib_cnt] >= n */
    while (fib_cnt <= 1 || fib[fib_cnt] < n) {
        /* ensure capacity */
        if (fib_cnt == fib_cap) {
            fib_cap = fib_cap ? fib_cap * 2 : 4;
            idx_t *tmp = (idx_t*)realloc(fib, fib_cap * sizeof(idx_t));
            if (!tmp) {
                free(fib);
                return 0;
            }
            fib = tmp;
        }
        idx_t next = fib[fib_cnt] + fib[fib_cnt - 1];
        fib_cnt++;
        fib[fib_cnt] = next;
    }

    /* find highest usable Fibonacci index */
    size_t hi = fib_cnt;
    while (fib[hi] > n)
        hi--;

    seq_t run_count = 0;
    bool in_run = false;
    idx_t remaining = n;

    for (idx_t i = hi; i > 0; i--) {
        if (remaining >= fib[i]) {
            remaining -= fib[i];
            in_run = false;
        } else {
            if (!in_run) {
                run_count++;
                in_run = true;
            }
        }
    }

    return run_count;
}
