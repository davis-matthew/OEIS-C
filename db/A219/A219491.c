/*
 * A219491  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 5, 3, 8, 11, 19, 30,
 *          ....
 * https://oeis.org/A219491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a sum of * distinct
 *            elements of the generalized Fibonacci sequence * 5, 3, 8, 11,
 *            19, 30, … (a(0)=5, a(1)=3, a(k)=a(k-1)+a(k-2))
 * Range:     u8 n <= 1200, u16 n <= 8700, u32 n <= 8700, u64 n <= 8700,
 *            u128 n <= 8700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* calloc, free */

static seq_t *alloc_seq(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap]();
#else
    return (seq_t *)calloc(cap, sizeof(seq_t));
#endif
}

static void free_seq(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Generate the generalized Fibonacci numbers up to n. */
    size_t fib_cap = 16;               /* initial capacity */
    seq_t *fib = alloc_seq(fib_cap);
    if (!fib) return (seq_t)0;         /* not enough memory */

    fib[0] = (seq_t)5;
    fib[1] = (seq_t)3;
    size_t fib_cnt = 2;

    while (1) {
        seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        if (next > (seq_t)n) break;   /* no larger numbers needed */

        if (fib_cnt >= fib_cap) {      /* grow the array */
            size_t new_cap = fib_cap << 1;
            seq_t *tmp = alloc_seq(new_cap);
            if (!tmp) { free_seq(fib); return (seq_t)0; }
            for (size_t i = 0; i < fib_cnt; ++i) tmp[i] = fib[i];
            free_seq(fib);
            fib = tmp;
            fib_cap = new_cap;
        }
        fib[fib_cnt++] = next;
    }

    /* DP array for subset‑sum counts. */
    size_t dp_cap = (size_t)n + 1;
    seq_t *dp = alloc_seq(dp_cap);
    if (!dp) { free_seq(fib); return (seq_t)0; }
    dp[0] = (seq_t)1;            /* empty subset sums to zero */

    for (size_t idx = 0; idx < fib_cnt; ++idx) {
        seq_t value = fib[idx];
        idx_t val_idx = (idx_t)value;
        if (val_idx > n) continue;

        for (size_t s = n; s + 1 > (size_t)val_idx; --s) {
            dp[s] += dp[s - (size_t)val_idx];
        }
    }

    seq_t result = dp[(size_t)n];
    free_seq(dp);
    free_seq(fib);
    return result;
}
