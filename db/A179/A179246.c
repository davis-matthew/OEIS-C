/*
 * A179246  Numbers that have 6 terms in their Zeckendorf representation.
 * https://oeis.org/A179246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9843 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th smallest integer that has exactly 6 terms in its Zeckendorf representation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* generate all sums of 6 non‑consecutive Fibonacci numbers,
   where fib[0]=1, fib[1]=2; M must be large enough so that the
   output array has at least comb6(M-5) elements. */
static void generate_sums(idx_t M, const seq_t *fib, seq_t *out)
{
    size_t idx = 0;
    for (idx_t i0 = 0; i0 <= M - 11; ++i0)
        for (idx_t i1 = i0 + 2; i1 <= M - 9; ++i1)
            for (idx_t i2 = i1 + 2; i2 <= M - 7; ++i2)
                for (idx_t i3 = i2 + 2; i3 <= M - 5; ++i3)
                    for (idx_t i4 = i3 + 2; i4 <= M - 3; ++i4)
                        for (idx_t i5 = i4 + 2; i5 <= M - 1; ++i5)
                        {
                            out[idx++] = fib[i0] + fib[i1] + fib[i2]
                                         + fib[i3] + fib[i4] + fib[i5];
                        }
}

/* comparison function for qsort */
static int cmp_seqt(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

/* compute binomial coefficient C(n,6); n >= 6 */
static size_t comb6(idx_t n)
{
    if (n < 6) return 0;
    uint64_t c = 1;
    for (int k = 1; k <= 6; ++k)
        c = c * (n - k + 1) / k;
    return (size_t)c;
}

/* static cache of pre‑computed sums */
static idx_t        current_M   = 0;   /* largest Fibonacci index used so far   */
static seq_t *      sums        = NULL;/* sorted array of all generated sums    */
static size_t       sumcnt      = 0;   /* number of elements in sums            */

/* ensure that sums contains at least n entries */
static void ensure_precomputed(idx_t n)
{
    if (n <= sumcnt) return;          /* already sufficient */

    /* find minimal M with comb6(M-5) >= n */
    idx_t M = current_M ? current_M : 25;  /* start with 25 terms */
    while (comb6(M - 5) < n) M += 5;

    /* free previous data if any */
    if (sums) mem_free(sums);

    /* compute Fibonacci numbers up to M-1 */
    seq_t *fib = mem_new(seq_t, M);
    if (!fib) exit(1);
    fib[0] = 1;
    fib[1] = 2;
    for (idx_t i = 2; i < M; ++i)
        fib[i] = fib[i-1] + fib[i-2];

    /* allocate array for all sums */
    size_t cnt = comb6(M - 5);
    seq_t *new_sums = mem_new(seq_t, cnt);
    if (!new_sums) exit(1);

    /* generate sums */
    generate_sums(M, fib, new_sums);

    /* sort them */
    mem_qsort(new_sums, cnt, sizeof(seq_t), cmp_seqt);

    /* store results */
    sums      = new_sums;
    sumcnt    = cnt;
    current_M = M;

    mem_free(fib);
}

/* OEIS entry function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n == 0 */
    ensure_precomputed(n);
    return sums[n - 1];              /* 1‑based indexing */
}
