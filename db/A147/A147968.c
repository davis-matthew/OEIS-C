/*
 * A147968  Long "if ladder" modulo 8 recursion: ( using f[n] for f(n)) f[n] =
 *          f[f[n - 1]] + If[n < 8, f[n - f[( n - 1)]], If[Mod[n, 8] == 0,
 *          f[f[n/8]], If[Mod[1 + n, 8] == 1, f[f[(n - 1)/8]], If[Mod[2 + n,
 *          8] == 2, f[f[(n - 2)/8]], If[Mod[3 + n, 8] == 3, f[f[(n - 3)/8]],
 *          If[ Mod[4 + n, 8] == 4, f[f[(n - 4)/...
 * https://oeis.org/A147968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A147968
    Recurrence:
        a(0) = 0
        a(1) = 1
        a(2) = 1
        a(3) = 2
        a(4) = 2
        a(5) = 3
        a(6) = 4
        a(7) = 4
        For n ≥ 1, let b = a(n‑1).
        a(n) = a(a(n‑1)) + add(n)
        where
          add(n) = a(n – b)                      if n < 8  or n % 8 ≠ 0
          add(n) = a(a(n/8))                     if n % 8 == 0
    The function below memoises computed values so that any ordering of
    queries works and each value is calculated only once.
*/

#include <stddef.h>
#include <stdlib.h>

/*  External linkage as required.  */
seq_t a(idx_t n)
{
    /*  Dynamic array storing memoised values.  */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t max_computed = 0;  /* maximum index that has been filled */

    /*  Ensure `cache` is large enough to store indices 0 … n.  */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /*  Initialise the first eight terms if they are not yet present.  */
    if (max_computed < 7) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)4;
        cache[7] = (seq_t)4;
        max_computed = 7;
    }

    /*  Compute any terms that have not yet been computed.  */
    for (size_t i = max_computed + 1; i <= n; ++i) {
        size_t i_minus_1 = i - 1;

        /*  a(a(i-1))  */
        size_t a_prev  = (size_t)cache[i_minus_1];
        seq_t base     = cache[ a_prev ];

        /*  the extra term */
        seq_t add;
        if (i < 8) {
            /*  a(i - a(i-1))  */
            size_t sub = i - a_prev;
            add = cache[sub];
        } else if (i % 8 == 0) {
            /*  a(a(i/8))  */
            size_t sub = i / 8;
            size_t a_sub = (size_t)cache[sub];
            add = cache[a_sub];
        } else {
            /*  fallback: a(i - a(i-1))  */
            size_t sub = i - a_prev;
            add = cache[sub];
        }

        cache[i] = base + add;
    }
    max_computed = n;

    return cache[(size_t)n];
}
