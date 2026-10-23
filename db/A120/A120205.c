/*
 * A120205  a(1) = 4; a(n) = floor((39 + Sum_{i=1..n-1} a(i))/9).
 * https://oeis.org/A120205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4; a(n) = floor((39 + Σ_{i=1}^{n-1} a(i)) / 9)
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (OEIS A120205) */

#include <stdlib.h>

/* Memoised recursive definition using a running sum */
seq_t a(idx_t n)
{
    /* Guard value for n < 1 – sequence is defined only for n ≥ 1 */
    if (n < (idx_t)1) return 0;

    /* Cache of already‐computed terms (0‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of terms stored */

    /* Running sum S = Σ_{i=1}^{k} a(i) for the current k */
    static seq_t sum = 0;
    static seq_t q = 0;         /* floor(sum / 9) */
    static seq_t r = 0;         /* sum mod 9 */

    /* Resize cache if necessary (doubling strategy) */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the term is already known, return it */
    if (n <= cache_size) return cache[n - 1];

    /* Compute terms up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* a = q + 4 + (r >= 6 ? 1 : 0)  */
        seq_t a_val = q + (seq_t)4 + ((r >= 6) ? (seq_t)1 : (seq_t)0);
        cache[i] = a_val;

        /* Update the running sum */
        sum += a_val;

        /* Update q and r from the new sum */
        q = sum / 9;
        r = sum % 9;
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
