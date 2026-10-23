/*
 * A165522  The number of 54321-avoiding separable permutations of length n.
 * https://oeis.org/A165522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165522: number of 54321‑avoiding separable permutations
 * ------------------------------------------
 * a(n) satisfies the 18‑term linear recurrence (from the denominator of
 * the generating function)
 *
 * a(n) = 18 a(n‑1) – 148 a(n‑2) + 743 a(n‑3) – 2564 a(n‑4) + 6488 a(n‑5)
 *        – 12536 a(n‑6) + 18999 a(n‑7) – 22992 a(n‑8) + 22474 a(n‑9)
 *        – 17876 a(n‑10) + 11622 a(n‑11) – 6189 a(n‑12) + 2697 a(n‑13)
 *        – 957 a(n‑14) + 273 a(n‑15) – 61 a(n‑16) + 10 a(n‑17) – a(n‑18)
 *
 * Initial values (a(0)…a(17)) are taken from the OEIS entry. */

/* Standard includes required for both C and C++ builds */
#include <stdlib.h>   /* malloc, realloc, abort */
#ifdef __cplusplus
#include <new>       /* std::nothrow (optional) */
#endif

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* number of elements stored */

    /* number of terms we need (indices 0..n inclusive) */
    size_t target = (size_t)n + 1;

    if (cache_len < target) {          /* grow cache if necessary */
        size_t old_len = cache_len;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        /* allocate new array with the requested size */
        new_cache = new seq_t[target];
        /* copy old values if any */
        for (size_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        /* free old array */
        delete[] cache;
#else
        /* C path: use realloc which handles copying properly for int types */
        new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failed */
#endif
        cache = new_cache;

        /* initialise the first 18 terms if they are not yet set */
        static const seq_t init[18] = {
            (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,  (seq_t)22,
            (seq_t)89, (seq_t)368, (seq_t)1488, (seq_t)5831, (seq_t)22311,
            (seq_t)84223, (seq_t)316181, (seq_t)1185884, (seq_t)4452567,
            (seq_t)16742230, (seq_t)63025805, (seq_t)237423928, (seq_t)894681874
        };
        if (target > old_len) {
            size_t i_start = old_len;
            if (i_start > 18) i_start = 18;   /* safety */
            for (size_t i = i_start; i < 18 && i < target; ++i)
                cache[i] = init[i];
        }

        /* compute all remaining terms using the recurrence */
        size_t start = old_len > 18 ? old_len : 18;
        for (size_t i = start; i < target; ++i) {
            seq_t val = 18 * cache[i - 1];
            val -= 148 * cache[i - 2];
            val += 743 * cache[i - 3];
            val -= 2564 * cache[i - 4];
            val += 6488 * cache[i - 5];
            val -= 12536 * cache[i - 6];
            val += 18999 * cache[i - 7];
            val -= 22992 * cache[i - 8];
            val += 22474 * cache[i - 9];
            val -= 17876 * cache[i -10];
            val += 11622 * cache[i -11];
            val -= 6189 * cache[i -12];
            val += 2697 * cache[i -13];
            val -= 957 * cache[i -14];
            val += 273 * cache[i -15];
            val -= 61 * cache[i -16];
            val += 10 * cache[i -17];
            val -= cache[i -18];
            cache[i] = val;
        }
        cache_len = target;
    }

    return cache[n];
}
