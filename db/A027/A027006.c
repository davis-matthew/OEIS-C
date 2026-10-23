/*
 * A027006  a(n) = T(2*n+1, n+3), T given by A026998.
 * https://oeis.org/A027006
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A027006
    a(n) = T(2*n+1, n+3)   (T from A026998)
    Recurrence (for n ≥ 7):
        a(n) = 8*a(n‑1) – 26*a(n‑2) + 45*a(n‑3) – 45*a(n‑4)
               + 26*a(n‑5) – 8*a(n‑6) + a(n‑7)
    Initial values (offset 2):
        a(2)=1, a(3)=34, a(4)=281, a(5)=1397, a(6)=5353,
        a(7)=17643, a(8)=53062
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding computed values */
static size_t cache_cap = 0;         /* capacity of the array        */
static idx_t   cache_max = 1;        /* highest index that has been
                                       computed (≥ 1)                */

/* initialise cache with the first several terms (up to index 8) */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialised */

    cache_cap = 16;                     /* at least up to index 8 */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL) abort();         /* out of memory */

    cache[0] = (seq_t)0;                 /* unused – any value OK */
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)34;
    cache[4] = (seq_t)281;
    cache[5] = (seq_t)1397;
    cache[6] = (seq_t)5353;
    cache[7] = (seq_t)17643;
    cache[8] = (seq_t)53062;

    cache_max = 8;
}

/* grow the cache so that index n can be accessed */
static void grow_cache(idx_t n)
{
    if (n < (idx_t)cache_cap) return;

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();           /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined: behaviour not required */

    init_cache();

    if (n > cache_max) {
        grow_cache(n);

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* recurrence using only seq_t arithmetic */
            seq_t val = cache[i-1] * (seq_t)8;
            val -= cache[i-2] * (seq_t)26;
            val += cache[i-3] * (seq_t)45;
            val -= cache[i-4] * (seq_t)45;
            val += cache[i-5] * (seq_t)26;
            val -= cache[i-6] * (seq_t)8;
            val += cache[i-7];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
