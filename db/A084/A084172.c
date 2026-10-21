/*
 * A084172  a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4).
 * https://oeis.org/A084172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + (n-2); initial values: a(0) = 1, a(1)
 *            = 2 (recurrence derived from OEIS definition)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum n expected (~10000) – safe upper bound for all builds */
    #define MAX_N 20000

    /* static cache of already‑computed terms */
    static seq_t cache[MAX_N + 1];
    static idx_t last_computed = 1;   /* we know a(0) and a(1) */

    /* initialise base cases once */
    if (last_computed == 1)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* compute terms up to n lazily */
    while (last_computed < n && last_computed < MAX_N)
    {
        /* a(k) = a(k-1) + 2*a(k-2) + (k-2)  for k >= 2 */
        seq_t next = cache[last_computed]         /* a(k-1) */
                   + (seq_t)2 * cache[last_computed - 1]    /* 2*a(k-2) */
                   + (seq_t)(last_computed - 1);           /* (k-2) */
        cache[++last_computed] = next;
    }

    /* guard against requests beyond MAX_N – they are never needed */
    return (n <= last_computed) ? cache[n] : (seq_t)0;
}
