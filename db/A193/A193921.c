/*
 * A193921  Triangular array: the fusion of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=sum{F(k+1)*x^(n-k) : 0<=k<=n}, where F=A000045 (Fibonacci
 *          numbers), and q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0}^{j} F(r - k), where n = r*(r+1)/2 + j (0 ≤ j ≤
 *            r), F(0)=0, F(1)=1 are the Fibonacci numbers
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* internal cache for Fibonacci numbers */
static seq_t *fib_cache = NULL;          /* array of Fibonacci numbers F(i) */
static idx_t fib_cache_len = 0;          /* length of fib_cache (indices 0..len-1) */

/* ensure that fib_cache contains Fibonacci numbers up to index `need` (inclusive) */
static void ensure_fib(idx_t need)
{
    if (fib_cache_len > need)
        return;                         /* already has enough */

    /* grow the array */
    idx_t new_len = fib_cache_len > 1 ? fib_cache_len * 2 : 1;
    while (new_len <= need)
        new_len *= 2;                    /* guarantee new_len > need */

    seq_t *tmp = (seq_t*)realloc(fib_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failed */
    fib_cache = tmp;

    /* compute new Fibonacci numbers */
    for (idx_t i = fib_cache_len; i < new_len; ++i) {
        if (i == 0)
            fib_cache[i] = (seq_t)0;
        else if (i == 1)
            fib_cache[i] = (seq_t)1;
        else
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_cache_len = new_len;
}

/* main function: return a(n) for the OEIS A193921 sequence */
seq_t a(idx_t n)
{
    /* determine the row number r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;            /* high can be n because r ≤ n */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        /* compute mid*(mid+1)/2 without overflow */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;                       /* row number */
    /* index inside the row */
    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    idx_t j = n - (idx_t)tri;

    /* special case: first row */
    if (r == 0)
        return (seq_t)1;

    /* ensure Fibonacci cache is large enough */
    ensure_fib(r);

    /* compute the sum of F(r) + F(r-1) + … + F(r-j) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= j; ++k)
        sum += fib_cache[r - k];

    return sum;
}
