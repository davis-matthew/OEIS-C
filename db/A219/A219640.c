/*
 * A219640  Numbers m for which there exists k such that m = k - (number of
 *          1's in Zeckendorf expansion of k); distinct values in A219641.
 * https://oeis.org/A219640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th distinct value of m = k -
 *            popcount_zeckendorf(k), with a(0)=0. The implementation
 *            precomputes Fibonacci numbers and builds the sequence on demand,
 *            keeping it sorted
 * Range:     u8 n <= 159, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   ensure_size_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_size_seq_t.h"

/*--- Helper: ensure Fibonacci table contains numbers ≥ k -----------------*/
static uint64_t *fib = NULL;
static size_t fib_cnt = 0;
static size_t fib_cap = 0;

static void ensure_fib(idx_t k)
{
    if (fib == NULL) {
        fib_cap = 4;
        fib = (uint64_t *)malloc(fib_cap * sizeof(uint64_t));
        if (!fib) abort();
        fib_cnt = 0;
    }
    while (fib_cnt == 0 || fib[fib_cnt - 1] < k) {
        /* grow array if needed */
        if (fib_cnt >= fib_cap) {
            fib_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(fib, fib_cap * sizeof(uint64_t));
            if (!tmp) abort();
            fib = tmp;
        }
        uint64_t next;
        if (fib_cnt == 0)
            next = 1;
        else if (fib_cnt == 1)
            next = 2;
        else {
            __uint128_t sum = (__uint128_t)fib[fib_cnt - 1] + (__uint128_t)fib[fib_cnt - 2];
            if (sum > (__uint128_t)~(uint64_t)0) break; /* overflow guard */
            next = (uint64_t)sum;
        }
        fib[fib_cnt++] = next;
    }
}

/*--- Helper: count bits in Zeckendorf representation of k ----------------*/
static idx_t popcount_zeckendorf(idx_t k)
{
    idx_t count = 0;
    idx_t remaining = k;
    /* traverse Fibonacci numbers from largest to smallest */
    for (idx_t i = fib_cnt; remaining > 0 && i > 0; --i) {
        idx_t idx = i - 1;
        if (fib[idx] <= remaining) {
            ++count;
            remaining -= fib[idx];
            if (idx > 0) i -= 1; /* skip the next Fibonacci to avoid consecutive usage */
        }
    }
    return count;
}

/*--- Main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *vals = NULL;          /* array of distinct m values */
    static size_t val_cnt = 0, val_cap = 0;
    static idx_t next_k = 0;            /* next k to process */

    /* extend the sequence until we have a(n) */
    while (val_cnt <= n) {
        /* make sure Fibonacci table covers current k */
        ensure_fib(next_k);

        /* compute m = k - popcount_zeckendorf(k) */
        idx_t pc = popcount_zeckendorf(next_k);
        seq_t m = (seq_t)(next_k - pc);

        /* append if m is new (sequence is non‑decreasing) */
        if (val_cnt == 0 || m > vals[val_cnt - 1]) {
            ensure_size_seq_t(&vals, &val_cap, val_cnt + 1);
            vals[val_cnt++] = m;
        }
        ++next_k;
    }
    return vals[n];
}
