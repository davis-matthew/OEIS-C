/*
 * A063894  Start with x, y; then concatenate each word in turn with all
 *          preceding words, getting x y xy xxy yxy xxxy yxxy xyxxy ...;
 *          sequence gives number of words of length n. Also binary trees by
 *          degree: x y (x,y) (x,(x,y)) (y,(x,y)) (x,(x,(x,y))) (y,(x,(x,y)))
 *          ((x,y),(x,(x,y)))...
 * https://oeis.org/A063894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1)=2; for n≥2, a(n) = Σ_{i=1}^{⌊(n-1)/2⌋} a(i)*a(n-i) + (n
 *            even ? (a(n/2)-1)*a(n/2)/2 : 0)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, calloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[i] holds a(i) */
    static idx_t max_computed = 0;       /* highest index computed */

    if (n == 0)                          /* out-of-range guard */
        return 0;

    /* allocate or grow cache */
    if (cache == NULL) {                 /* first call */
        cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;             /* a(1) */
        max_computed = 1;
    }
    else if (n > max_computed) {         /* need more space */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        /* new indices will be filled below */
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i - 1) / 2;                       /* floor((i-1)/2) */
        for (idx_t j = 1; j <= limit; ++j) {
            sum += cache[j] * cache[i - j];
        }
        if (i & 1) {  /* odd i → no extra term */
            cache[i] = sum;
        } else { /* even i → add (a(i/2)-1)*a(i/2)/2 */
            seq_t k = cache[i / 2];
            cache[i] = sum + (k - (seq_t)1) * k / (seq_t)2;
        }
    }

    max_computed = n;
    return cache[n];
}
