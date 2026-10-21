/*
 * A228025  a(1) = least k such that 1/2+1/3+1/4+1/5 < H(k) - H(5); a(2) =
 *          least k such that H(a(1)) - H(5) < H(k) -H(a(1)), and for n > 2,
 *          a(n) = least k such that H(a(n-1)) - H(a(n-2)) > H(k) - H(a(n-1)),
 *          where H = harmonic number.
 * https://oeis.org/A228025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) for OEIS A228025
   Recurrence (for n ≥ 4):
      a(1) = 20,  a(2) = 76,  a(3) = 285
      a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3)
   (All arithmetic is in type seq_t, so values wrap naturally if they
   exceed the bit‑width of seq_t.) */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;     /* allocated size of cache */
    static idx_t  computed = 0;   /* highest index that has been filled (1‑based) */

    /* ensure sufficient storage for n */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)20;
            } else if (i == 2) {
                cache[1] = (seq_t)76;
            } else if (i == 3) {
                cache[2] = (seq_t)285;
            } else {
                cache[i-1] = 5 * cache[i-2] - 5 * cache[i-3] + cache[i-4];
            }
        }
        computed = n;
    }

    return cache[n-1];
}
