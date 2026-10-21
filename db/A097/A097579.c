/*
 * A097579  Triangular numbers k such that 2*k-1 is also a triangular number.
 * https://oeis.org/A097579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(n) = 35*a(n-1) – 35*a(n-2) + a(n-3) for n ≥ 4 with
 *            a(1) = 1, a(2) = 28, a(3) = 946. This is the sequence of
 *            triangular numbers k for which 2*k‑1 is also triangular
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based array holding already computed terms   */
    static idx_t  cached = 0;      /* highest index that has been computed so far   */
    static size_t cap = 0;         /* allocated size of cache (in terms, not bytes) */

    if (n == 0)          /* the offset of this sequence is 1 – behaviour for n=0 is irrelevant */
        return (seq_t)0;

    /* Ensure the cache has room for index n (we also keep a dummy slot at index 0) */
    if (n > cap)
    {
        size_t new_cap = (cap == 0) ? 4 : cap * 2;
        while (new_cap < n + 1) new_cap *= 2;          /* safety margin */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* allocation failure – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first three terms on the first call */
    if (cached < 3)
    {
        cache[1] = (seq_t)1;      /* a(1) */
        cache[2] = (seq_t)28;     /* a(2) */
        cache[3] = (seq_t)946;    /* a(3) */
        cached = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i)
    {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        cache[i] = (seq_t)35 * t1 - (seq_t)35 * t2 + t3;
    }
    cached = n;

    return cache[n];
}
