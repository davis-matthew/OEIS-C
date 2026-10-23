/*
 * A194377  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(6) and < > denotes fractional part.
 * https://oeis.org/A194377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that Sum_{k=1..m}
 *            ({k*sqrt(6)+1/2} - {k*sqrt(6)}) > 0
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The test {k*sqrt(6)} < 1/2 is performed by:  
   let m = floor(sqrt(6*k^2))  
   then {k*sqrt(6)} < 0.5  ⇔ 24*k^2 < 4*m^2 + 4*m + 1       */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/int_sqrt.h"

/* dynamic cache of the found terms */
static seq_t *memo      = NULL;   /* array of terms */
static size_t memo_size = 0;     /* number of terms stored */
static size_t memo_cap  = 0;     /* allocated capacity */

/* state for generating terms incrementally */
static idx_t    next_k = 1;   /* next k to consider */
static long long D      = 0;   /* 2*A(k)-k, signed */

/* ensure memo has capacity at least 'needed' */
static void ensure_cache_capacity(size_t needed)
{
    if (memo_cap >= needed) return;
    size_t new_cap = memo_cap ? memo_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!p) abort();               /* out of memory */
    memo      = p;
    memo_cap  = new_cap;
}

/* test whether {k*sqrt(6)} < 1/2 using only integer arithmetic */
static bool less_than_half(idx_t k)
{
    idx_t k2 = k * k;               /* k^2   */
    idx_t N  = 6 * k2;              /* 6*k^2 */
    idx_t m  = int_sqrt(N);         /* floor(sqrt(6*k^2)) */

    /* left side: 24*k^2   */
    idx_t left  = 24 * k2;
    /* right side: 4*m^2 + 4*m + 1   */
    idx_t right = 4 * m * m + 4 * m + 1;

    return left < right;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but defensive */

    /* Generate terms until we have at least n of them. */
    while (memo_size < (size_t)n)
    {
        bool lt = less_than_half(next_k);
        D += lt ? 1 : -1;          /* update D = 2*A(k) - k */

        if (D > 0)                /* condition satisfied for this m */
        {
            ensure_cache_capacity(memo_size + 1);
            memo[memo_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return memo[n - 1];
}
