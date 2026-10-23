/*
 * A100674  a(1) = 1; a(n+1) = Sum_{k=1..n} a(gcd(k, a(n))).
 * https://oeis.org/A100674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = sum_{k=1}^{n-1} a(gcd(k, a(n-1)))
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compile with both C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Euclidean GCD for seq_t values (works for arbitrary‑precision integer classes) */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* memoised computation of a(n) */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n <= 0 */

    /* static table storing computed values; arr[0] is unused */
    static seq_t *arr   = NULL;
    static idx_t  size  = 0;   /* allocated number of elements (>= 2 when non‑NULL) */
    static idx_t  max_n = 0;   /* highest n that has been computed */

    /* initial allocation on first call */
    if (arr == NULL)
    {
        size = (idx_t)2;               /* at least positions 0 and 1 */
        arr  = seq_malloc(size);
        if (!arr) return (seq_t)0;     /* allocation failure handled silently */
        arr[(idx_t)1] = (seq_t)1;      /* a(1) = 1 */
        max_n = (idx_t)1;
    }

    /* grow table if necessary */
    if (n + (idx_t)1 > size)
    {
        idx_t new_size = size * (idx_t)2;
        if (new_size < n + (idx_t)1) new_size = n + (idx_t)1;
        seq_t *tmp = seq_realloc(arr, new_size);
        if (!tmp) return (seq_t)0;
        arr    = tmp;
        size   = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + (idx_t)1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        seq_t prev = arr[i - (idx_t)1];
        for (idx_t k = (idx_t)1; k < i; ++k)
        {
            seq_t g = my_gcd((seq_t)k, prev);
            idx_t idx = (idx_t)g;   /* g <= k < i, thus fits into idx_t */
            sum += arr[idx];
        }
        arr[i] = sum;
    }
    max_n = n;
    return arr[n];
}
