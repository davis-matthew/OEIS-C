/*
 * A156713  Positive numbers y such that y^2 is of the form x^2+(x+16807)^2
 *          with integer x.
 * https://oeis.org/A156713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-11) - a(n-22) for n > 22 Initial values (n =
 *            1..22): 12005, 12467, 12985, 14063, 15025, 16807, 19073, 20923,
 *            24157, 26747, 31213, 40817, 48055, 53753, 63455, 71077, 84035,
 *            99413, 111475, 131957, 148015, 175273
 * Range:     u8 none, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   mul6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul6.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* undefined, but safe */

    /* static cache: 1‑indexed, grows as needed          */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;              /* allocated indices 0..cache_size */
    static idx_t computed_max = 0;            /* highest index with valid data */

    /* allocate and initialise first 22 terms once           */
    if (cache == NULL) {
        cache_size = 22;                      /* indices 0..22 */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) exit(1);                  /* allocation failure */
        cache[0] = 0;                         /* unused */

        /* initial 22 terms (stored as 64‑bit so no overflow) */
        static const idx_t init[22] = {
            12005ULL, 12467ULL, 12985ULL, 14063ULL, 15025ULL, 16807ULL,
            19073ULL, 20923ULL, 24157ULL, 26747ULL, 31213ULL, 40817ULL,
            48055ULL, 53753ULL, 63455ULL, 71077ULL, 84035ULL, 99413ULL,
            111475ULL, 131957ULL, 148015ULL, 175273ULL
        };
        for (idx_t i = 1; i <= 22; ++i)
            cache[i] = (seq_t)init[i - 1];

        computed_max = 22;
    }

    /* enlarge cache if n exceeds current size            */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms only once                      */
    if (n > computed_max) {
        idx_t start = (computed_max < 22) ? 23 : computed_max + 1;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = mul6(cache[i - 11]) - cache[i - 22];
        }
        computed_max = n;
    }

    return cache[n];
}
