/*
 * A039062  Numbers whose base-8 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑8 representation contains an
 *            equal number of digits 2 and 3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   grow_seq, has_equal_2_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/has_equal_2_3.h"

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_search = 0;

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                     /* offset starts at 1 */

    /* Ensure cache can hold at least n entries */
    if ((size_t)n > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Generate terms until we reach n */
    while (cache_len < (size_t)n) {
        /* Find next candidate with equal 2s and 3s */
        seq_t cand = next_search;
        while (!has_equal_2_3(cand))
            ++cand;

        /* Store it */
        cache[cache_len++] = cand;
        next_search = cand + 1;        /* start searching from the next integer */
    }

    return cache[n - 1];
}
