/*
 * A090328  Number of rules of a context-free grammar in Chomsky normal form
 *          that generates all permutations of n symbols.
 * https://oeis.org/A090328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A090328
    a(n) = a(n‑1)*5 - a(n‑2)*7 + a(n‑3)*3   (n ≥ 4)
    a(1)=1, a(2)=4, a(3)=12
*/
#include <stddef.h>

#define MAX_CACHE 10001   /* 1‑based array: indices 1 … MAX_CACHE are cached */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_CACHE+1];
    static size_t cached_len = 0;  /* highest n for which cache[n] is valid */

    /* Offset 1 – nothing requested for n==0 in normal use */
    if (n == 0)
        return (seq_t)0;

    /* Base values for n = 1..3 */
    if (n <= 3) {
        switch ((int)n) {
            case 1:  return (seq_t)1;
            case 2:  return (seq_t)4;
            case 3:  return (seq_t)12;
            default: return (seq_t)0;  /* unreachable */
        }
    }

    /* Initialise first three values once */
    if (cached_len < 3) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cached_len = 3;
    }

    /* If the requested value is within the cached range, compute it */
    if ((size_t)n <= MAX_CACHE) {
        if (n > cached_len) {
            for (size_t i = cached_len + 1; i <= (size_t)n; ++i) {
                /* a(i) = 5*a(i‑1) - 7*a(i‑2) + 3*a(i‑3)
                   compute as a sum of positives before the subtraction
                   to avoid unsigned underflow                               */
                cache[i] = cache[i-1] * 5 + cache[i-3] * 3 - cache[i-2] * 7;
            }
            cached_len = (size_t)n;
        }
        return cache[(size_t)n];
    }

    /* For n larger than the cache size, recompute on the fly
       using the same recurrence; this will work for all
       integer types that can hold the intermediate results.   */
    seq_t a1 = cache[1], a2 = cache[2], a3 = cache[3];
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a = a3 * 5 + a1 * 3 - a2 * 7;
        a1 = a2;
        a2 = a3;
        a3 = a;
    }
    return a3;
}
