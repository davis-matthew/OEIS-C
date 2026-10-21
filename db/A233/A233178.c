/*
 * A233178  Number of 5Xn 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabelled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            192*a(n‑1)+31744*a(n‑2)-3178496*a(n‑3)-170917888*a(n‑4)+13992198144*a(n‑5)+99857989632*a(n‑6)-17660905521152*a(n‑7)+211106232532992*a(n‑8)+2533274790395904*a(n‑9)-40532396646334464*a(n‑10)
 *            for n>11, with initial terms a(1)=236, a(2)=10496, a(3)=3497984,
 *            a(4)=808583168, a(5)=221463445504, a...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* First 11 terms as strings for arbitrary-precision types */
    static const char *first_str[11] = {
        "236",
        "10496",
        "3497984",
        "808583168",
        "221463445504",
        "56275748519936",
        "14822434649669632",
        "3838772637371203584",
        "1002338782428226650112",
        "260695613898360688214016",
        "67932875090988311936761856"
    };

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of terms currently stored */

    /* Ensure cache has at least capacity 11 for the initial values */
    if (cache == NULL) {
        cache_cap = (n > 11 ? n : 11);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Fill in the first 11 terms if not done yet */
    if (cache_len < 11) {
        for (size_t i = cache_len; i < 11; ++i)
            cache[i] = seq_from_str(first_str[i]);
        cache_len = 11;
    }

    /* If the requested term is already known, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms using the recurrence (only for n > 11) */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        pos_sum += 192ULL * cache[k - 2];
        pos_sum += 31744ULL * cache[k - 3];
        neg_sum += 3178496ULL * cache[k - 4];
        neg_sum += 170917888ULL * cache[k - 5];
        pos_sum += 13992198144ULL * cache[k - 6];
        pos_sum += 99857989632ULL * cache[k - 7];
        neg_sum += 17660905521152ULL * cache[k - 8];
        pos_sum += 211106232532992ULL * cache[k - 9];
        pos_sum += 2533274790395904ULL * cache[k - 10];
        neg_sum += 40532396646334464ULL * cache[k - 11];

        cache[k - 1] = pos_sum - neg_sum;
    }

    cache_len = n;
    return cache[n - 1];
}
