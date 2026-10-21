/*
 * A216624  Square array read by antidiagonals, T(n,k) = sum_{c|n,d|k}
 *          gcd(c,d) for n>=1, k>=1.
 * https://oeis.org/A216624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(i,j), where (i,j) are obtained from the antidiagonal
 *            enumeration * of the square array T. T(i,j) = sum_{c|i , d|j}
 *            gcd(c,d). Index n * (1‑based) is mapped to coordinates (i,j) by:
 *            * find smallest d such that d*(d+1)/2 >= n; * let k = n -
 *            (d-1)*d/2; // 1 ≤ k ≤ d * i = k; j = d - k + 1....
 * Range:     u8 n <= 2508, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"     /* gcd(seq_t, seq_t) */

/* Maximum number of divisors we will store for a single number.
   For n <= 10^9 this is more than sufficient (max ~1000). */
#define MAX_DIVS 2048

static inline idx_t triangle(idx_t d)
{
    /* d*(d+1)/2 with 128‑bit intermediate to avoid overflow */
    return (idx_t)((__int128)d * (d + 1) / 2);
}

/* Return all divisors of n in arr, and their count in *cnt.
   arr length must be at least MAX_DIVS. */
static void get_divisors(idx_t n, seq_t arr[MAX_DIVS], size_t *cnt)
{
    size_t k = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (k < MAX_DIVS) arr[k++] = (seq_t)d;
            idx_t other = n / d;
            if (other != d && k < MAX_DIVS) arr[k++] = (seq_t)other;
        }
    }
    *cnt = k;
}

/* Binary search to find the antidiagonal number d such that
   d*(d+1)/2 >= n. */
static idx_t find_diag(idx_t n)
{
    idx_t low = 1, high = 1;
    while (triangle(high) < n)
        high *= 2;

    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (triangle(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Map n to (i,j) on the antidiagonal enumeration */
    idx_t d = find_diag(n);
    idx_t tri_prev = triangle(d - 1);
    idx_t k = n - tri_prev;            /* position on diagonal d (1‑based) */
    idx_t i = k;
    idx_t j = d - k + 1;

    /* Enumerate divisors of i and j */
    seq_t divi[MAX_DIVS], divj[MAX_DIVS];
    size_t cnt_i, cnt_j;
    get_divisors(i, divi, &cnt_i);
    get_divisors(j, divj, &cnt_j);

    /* Sum gcd over all pairs of divisors */
    seq_t sum = 0;
    for (size_t a = 0; a < cnt_i; ++a)
    {
        seq_t c = divi[a];
        for (size_t b = 0; b < cnt_j; ++b)
            sum += gcd(c, divj[b]);
    }
    return sum;
}
