/*
 * A118892  Number of binary sequences of length n containing exactly one
 *          subsequence 0110.
 * https://oeis.org/A118892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n-4)*a(n)=2*(n-3)*a(n-1)-(n-1)*a(n-3)+n*a(n-4)  
 * Initial terms: a(0)=a(1)=a(2)=a(3)=0, a(4)=1 */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;     /* dynamic array of computed terms       */
static size_t cache_len = 0;    /* number of elements currently stored   */

/* Ensure that cache has entries for indices 0 .. n inclusive               */
static void ensure_cache(idx_t n) {
    size_t need = (size_t)n + 1;
    if (need <= cache_len) return;

    seq_t *newmem = (seq_t *)malloc(need * sizeof(seq_t));
    if (!newmem) abort();   /* allocation failure                            */

    if (cache) {
        memcpy(newmem, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    for (size_t i = cache_len; i < need; ++i) {
        if (i <= 3) {
            newmem[i] = 0;
        } else if (i == 4) {
            newmem[i] = 1;
        } else {
            /* Compute a(i) using the recurrence */
            seq_t term1 = 2 * ((seq_t)(i - 3) * newmem[i - 1]);
            seq_t term2 = ((seq_t)(i - 1)) * newmem[i - 3];
            seq_t term3 = (seq_t)i * newmem[i - 4];
            seq_t numerator = term1 - term2 + term3;
            seq_t denominator = (seq_t)(i - 4);
            newmem[i] = numerator / denominator;
        }
    }
    cache_len = need;
    cache = newmem;
}

seq_t a(idx_t n) {
    if (n <= 3) return 0;  /* explicit for safety                     */
    if (n == 4) return 1;
    ensure_cache(n);
    return cache[(size_t)n];
}
