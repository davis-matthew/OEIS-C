/*
 * A164316  Number of binary strings of length n with no substrings equal to
 *          000, 001, or 010.
 * https://oeis.org/A164316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4; for n>2 a(n) = 4 + ∑_{i=0}^{n-3} a(i)
 *            (prefix sum of previous terms)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache   = NULL;      /* sequence values */
    static seq_t *pref_cache = NULL;      /* prefix sums: pref[i] = Σ_{k=0}^{i} a(k) */
    static idx_t cache_size = 0;          /* number of elements stored */

    /* ----- initialise storage for first three terms ----- */
    if (!a_cache) {
        size_t init = 3;   /* indices 0,1,2 */
        a_cache   = (seq_t*)malloc(sizeof(seq_t)*init);
        pref_cache = (seq_t*)malloc(sizeof(seq_t)*init);
        if (!a_cache || !pref_cache) return 0;   /* allocation failed */

        a_cache[0]   = (seq_t)1;   pref_cache[0] = (seq_t)1;
        a_cache[1]   = (seq_t)2;   pref_cache[1] = (seq_t)3;      /*1+2*/
        a_cache[2]   = (seq_t)4;   pref_cache[2] = (seq_t)7;      /*1+2+4*/
        cache_size = 3;
    }

    /* ----- extend cache if needed ----- */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp_a   = (seq_t*)realloc(a_cache,   sizeof(seq_t)*new_size);
        seq_t *tmp_pref= (seq_t*)realloc(pref_cache,sizeof(seq_t)*new_size);
        if (!tmp_a || !tmp_pref) return 0;    /* allocation failed */
        a_cache = tmp_a;
        pref_cache = tmp_pref;

        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val;
            /* for i >= 3 the general formula applies */
            val = (seq_t)4 + pref_cache[i - 3];
            a_cache[i] = val;
            pref_cache[i] = pref_cache[i - 1] + val;
        }
        cache_size = new_size;
    }

    return a_cache[n];
}
