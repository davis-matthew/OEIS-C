/*
 * A074584  Esanacci (hexanacci or "6-anacci") numbers.
 * https://oeis.org/A074584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Esanacci numbers (A074584)
   Formula: a(0)=6, a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31, a(6)=63
   for n ≥ 7 : a(n) = 2*a(n‑1) – a(n‑7)   (uses the 6‑term recurrence)   */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* --- Memoisation array ------------------------------------------ */
    static seq_t *cache = NULL;   /* dynamic array for computed terms */
    static idx_t cap   = 0;       /* allocated length (in terms)   */
    static idx_t len   = 0;       /* number of terms currently stored */

    /* --- Initialise cache on first call ----------------------------- */
    if (cache == NULL) {
        cap  = 16;                       /* arbitrary start size     */
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failed        */

        /* initial terms a(0) … a(5) */
        cache[0] = 6;   cache[1] = 1;
        cache[2] = 3;   cache[3] = 7;
        cache[4] = 15;  cache[5] = 31;
        cache[6] = 63;  /* a(6) is the 6‑term sum of the previous 6 terms */
        len   = 7;                        /* we have terms 0 … 6           */
    }

    /* --- Return if already computed ------------------------------- */
    if (n < len) return cache[n];

    /* --- Compute terms up to n ------------------------------------ */
    while (len <= n) {
        /* grow cache if necessary */
        if (len >= cap) {
            idx_t newcap = cap * 2;
            if (newcap == 0) newcap = 16;
            seq_t *tmp = (seq_t *)realloc(cache,
                                           (size_t)newcap * sizeof(seq_t));
            if (tmp == NULL) return 0;  /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        seq_t val;
        if (len >= 7) {                /* use linear recurrence */
            val = cache[len - 1];
            val += cache[len - 1];     /* 2*a(n-1) */
            val -= cache[len - 7];     /* -a(n-7)  */
        } else {                      /* compute a(6) = sum of previous 6 terms */
            val = 0;
            for (idx_t i = 0; i < 6; ++i)
                val += cache[len - 1 - i];
        }
        cache[len++] = val;
    }

    return cache[n];
}
