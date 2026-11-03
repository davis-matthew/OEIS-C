/*
 * A205840  [s(k)-s(j)]/2, where the pairs (k,j) are given by A205837 and
 *          A205838.
 * https://oeis.org/A205840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th value of [(s(k)-s(j))/2] for pairs (k,j) where
 *            k>j, s(k)-s(j) is even, and s(n)=Fibonacci(n+1). The pairs are
 *            taken in lexicographic order of k then j, omitting pairs whose
 *            difference is odd
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Count how many j in [1,k-1] give an even difference with k.           */
static inline idx_t count_valid_j(idx_t k)
{
    if (k <= 2) return 0;
    idx_t m = k - 1;           /* number of candidates j */
    idx_t cnt_rem2 = 0;        /* j ≡ 2 (mod 3) */
    if (2 <= m)
        cnt_rem2 = ((m - 2) / 3) + 1;
    if (k % 3 == 2)               /* s(k) is even, so j must be even */
        return cnt_rem2;
    else                           /* s(k) is odd, j cannot be ≡2 (mod3) */
        return m - cnt_rem2;
}

/* Fibonacci cache -------------------------------------------------------- */
static seq_t *fib_cache   = NULL;   /* stores F[0..] (F(0)=0, F(1)=1) */
static size_t fib_capacity = 0;
static idx_t last_fib_idx   = 0;

/* Ensure the cache can hold Fibonacci numbers up to 'needed' (size)   */
static void ensure_fib_capacity(size_t needed)
{
    if (fib_capacity < needed)
    {
        size_t newcap = fib_capacity ? fib_capacity << 1 : 2;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, fib_cache, newcap);
        if (!tmp) abort();
        fib_cache   = tmp;
        fib_capacity = newcap;
    }
}

/* Return Fibonacci number F[idx] (F(0)=0, F(1)=1).                          */
static seq_t get_fib(idx_t idx)
{
    if (last_fib_idx < idx)
    {
        if (fib_capacity == 0)
        {
            fib_capacity = 2;
            fib_cache = mem_new(seq_t, fib_capacity);
            if (!fib_cache) abort();
            fib_cache[0] = (seq_t)0;
            fib_cache[1] = (seq_t)1;
            last_fib_idx = 1;
        }
        size_t needed = (size_t)idx + 1;
        ensure_fib_capacity(needed);
        for (idx_t i = last_fib_idx + 1; i <= idx; ++i)
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        last_fib_idx = idx;
    }
    return fib_cache[idx];
}

/* Return the j-th valid j for a given k (1‑based index).                 */
static inline idx_t find_j(idx_t k, idx_t idx)
{
    if (k % 3 == 2)
        return 2 + ((idx - 1) * 3);
    else
        return (3 * idx) / 2;    /* floor division */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n=0, though not used */

    idx_t sum = 0;
    for (idx_t k = 3; ; ++k)       /* start at k=3, as no pair exists for k<3 */
    {
        idx_t cnt = count_valid_j(k);
        if (sum + cnt >= n)
        {
            idx_t j_index = n - sum;                     /* 1‑based */
            idx_t j       = find_j(k, j_index);

            seq_t sk = get_fib(k + 1);  /* s(k) = F(k+1) */
            seq_t sj = get_fib(j + 1);  /* s(j) = F(j+1) */

            seq_t diff = sk - sj;       /* guaranteed even */
            return diff / 2;
        }
        sum += cnt;
    }
}
