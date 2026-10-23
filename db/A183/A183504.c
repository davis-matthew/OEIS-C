/*
 * A183504  Number of nX2 0..3 arrays with no element equal to the sum mod 4
 *          of its horizontal and vertical neighbors.
 * https://oeis.org/A183504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n‑1)+81*a(n‑4)-729*a(n‑5)+81*a(n‑8)-729*a(n‑9)
 *            -6561*a(n‑12)+59049*a(n‑13) with initial values a(1)=12,
 *            a(2)=81, a(3)=776, a(4)=6561, a(5)=59292, a(6)=531441,
 *            a(7)=4786704, a(8)=43046721, a(9)=387440172, a(10)=3486784401,
 *            a(11)=31381355016, a(12)=282429536481, a(13)=2541867422652
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding computed values; cache[i] stores a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of entries currently stored */

    /* initial values for a(1) … a(13) */
    static const seq_t init[13] = {
        12ULL, 81ULL, 776ULL, 6561ULL, 59292ULL,
        531441ULL, 4786704ULL, 43046721ULL, 387440172ULL,
        3486784401ULL, 31381355016ULL, 282429536481ULL,
        2541867422652ULL
    };

    /* first time: allocate cache, fill initial terms */
    if (cache_size == 0) {
        /* we need indices 0…13; index 0 is unused */
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (!cache) return 0;          /* unrecoverable allocation failure */

        cache[0] = 0;                  /* unused */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init[i - 1];
        cache_size = 14;               /* next free index */
    }

    /* extend cache up to requested n (if needed) */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* unrecoverable allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term = 0;

            /* a(n) = 9*a(n‑1)+81*a(n‑4)-729*a(n‑5)+81*a(n‑8)
                      -729*a(n‑9)-6561*a(n‑12)+59049*a(n‑13) */
            term += 9ULL       * cache[i - 1];
            term += 81ULL      * cache[i - 4];
            term -= 729ULL     * cache[i - 5];
            term += 81ULL      * cache[i - 8];
            term -= 729ULL     * cache[i - 9];
            term -= 6561ULL    * cache[i - 12];
            term += 59049ULL   * cache[i - 13];

            cache[i] = term;
        }
        cache_size = new_size;
    }

    /* index 0 is unused; behavior for n<1 is not required */
    if (n == 0)
        return 0;
    return cache[n];
}
