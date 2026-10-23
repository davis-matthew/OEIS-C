/*
 * A022878  a(n) = [ a(n-1)/a(1) ] + [ a(n-3)/a(3) ] + [ a(n-5)/a(5) ] + ...,
 *          for n >= 3.
 * https://oeis.org/A022878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3 a(n) = ⌊a(n-1)/a(1)⌋ + ⌊a(n-3)/a(3)⌋ +
 *            ⌊a(n-5)/a(5)⌋ + … for n ≥ 3
 * Range:     u8 n <= 22, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms (1‑based indices) */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t  len = 0;        /* number of terms currently cached */

    if (n == 0) return 0; /* undefined for n<1, defensive return */

    /* Extend capacity if needed */
    if ((idx_t)cap < n) {
        size_t new_cap = (cap > 0 ? cap * 2 : 4);
        while (new_cap < (size_t)n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();          /* fatal if allocation fails */
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {                 /* initial terms */
            cache[0] = 1;
            continue;
        }
        if (i == 2) {
            cache[1] = 3;
            continue;
        }

        seq_t sum = 0;
        /* Sum over odd k: 1,3,5,… with i-k ≥ 1 */
        for (idx_t k = 1; i >= k + 1; k += 2) {
            size_t num_idx = (size_t)(i - k - 1);  /* a(i-k) */
            size_t den_idx = (size_t)(k - 1);      /* a(k)   */
            sum += cache[num_idx] / cache[den_idx]; /* floor division */
        }
        cache[i - 1] = sum;
    }
    len = n;   /* cache now contains terms up to n */

    return cache[n - 1];
}
