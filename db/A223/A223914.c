/*
 * A223914  Number of n X 4 0..2 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A223914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A223914
    Recurrence: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
                 + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
    Initial values for 1 ≤ n ≤ 10 are given below.
*/

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* 10 known initial terms (index 1 based)  */
    static const seq_t init[10] = {
        (seq_t)46,   (seq_t)548,   (seq_t)3311,   (seq_t)14123,  (seq_t)48182,
        (seq_t)139925,(seq_t)359344,(seq_t)837243,(seq_t)1802306,(seq_t)3633256
    };

    if (n <= 10) return init[n-1];

    /* ----- common cache handling ----- */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;          /* number of terms stored (max index) */

    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 32;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first 10 terms on first use */
    if (len == 0) {
        for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
        len = 10;
    }

    /* ----- recurrence computation ----- */
#ifdef __cplusplus
    /* C++ build – seq_t may be an arbitrary‑precision class.
       Use only seq_t arithmetic; no intermediate overflow.               */
    for (idx_t k = (idx_t)len + 1; k <= n; ++k) {
        cache[k-1] =
            ( seq_t)9   * cache[k-2]  /* a(k-1) */
          - ( seq_t)36  * cache[k-3]  /* a(k-2) */
          + ( seq_t)84  * cache[k-4]  /* a(k-3) */
          - ( seq_t)126 * cache[k-5]  /* a(k-4) */
          + ( seq_t)126 * cache[k-6]  /* a(k-5) */
          - ( seq_t)84  * cache[k-7]  /* a(k-6) */
          + ( seq_t)36  * cache[k-8]  /* a(k-7) */
          - ( seq_t)9   * cache[k-9]  /* a(k-8) */
          +          cache[k-10];     /* a(k-9) */
    }
#else
    /* C build – seq_t is a built‑in integer type.
       Use a 128‑bit intermediate to avoid overflow.                       */
    for (idx_t k = (idx_t)len + 1; k <= n; ++k) {
        __int128 sum = 0;
        sum += ( __int128)9   * ( __int128)cache[k-2];
        sum -= ( __int128)36  * ( __int128)cache[k-3];
        sum += ( __int128)84  * ( __int128)cache[k-4];
        sum -= ( __int128)126 * ( __int128)cache[k-5];
        sum += ( __int128)126 * ( __int128)cache[k-6];
        sum -= ( __int128)84  * ( __int128)cache[k-7];
        sum += ( __int128)36  * ( __int128)cache[k-8];
        sum -= ( __int128)9   * ( __int128)cache[k-9];
        sum +=          ( __int128)cache[k-10];
        cache[k-1] = (seq_t)sum;
    }
#endif

    len = n;
    return cache[n-1];
}
