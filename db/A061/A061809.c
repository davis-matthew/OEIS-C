/*
 * A061809  When cubed gives number composed just of the digits 1, 2, 3, 4.
 * https://oeis.org/A061809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   cube_has_only_1to4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A061809 – numbers whose cube consists only of the digits 1,2,3,4.
 * The function builds the sequence on demand.  No formula or closed
 * form is known, so a search is performed until the request is
 * satisfied.
 */

/*  Required includes for C11 */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */
#include "../utils/cube_has_only_1to4.h"

seq_t a(idx_t n)
{
    /*  cache of discovered terms (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /*  current integer being tested for the cube */
    static uint64_t current = 0;

    /*  enlarge the cache until it holds the requested nth term   */
    while (cache_len < (size_t)n) {
        ++current;
        /*  compute current^3 using 128‑bit to avoid overflow   */
        __uint128_t cube = (__uint128_t)current * current * current;

        /*  we cannot directly call cube_has_only_1to4 with 128‑bit, so
         *  test the digits manually using the 128‑bit value.   */
        if (cube != 0) {
            __uint128_t temp = cube;
            bool ok = true;
            while (temp != 0) {
                uint8_t d = (uint8_t)(temp % 10);
                if (d < 1 || d > 4) { ok = false; break; }
                temp /= 10;
            }
            if (ok) {
                /*  store the term into the cache   */
                if (cache_len >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;   /* allocation failure, return 0 */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len] = (seq_t)current;
                ++cache_len;
            }
        }
    }

    return cache[n - 1];
}
