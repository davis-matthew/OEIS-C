/*
 * A164911  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence from the generating function a(0)=1, a(1)=20,
 *            a(2)=380, a(3)=7220, a(4)=137180, a(5)=2606420, a(6)=49521980,
 *            a(7)=940917620 For n ≥ 8: a(n) = 18*(a(n‑1)+…+a(n‑7)) –
 *            171*a(n‑8) + b(n) where b(n)=1 for n=8, b(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t init_vals[8] = {
    1ULL,   20ULL,  380ULL,  7220ULL,  137180ULL,
    2606420ULL, 49521980ULL, 940917620ULL
};

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)        /* memory allocation failed – return 0 */
        return (seq_t)0;
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 7; ++k)
                sum += cache[i - k];
            seq_t value = 18 * sum - 171 * cache[i - 8];
            if (i == 8)            /* add the special term b(8)=1 */
                value += 1;
            cache[i] = value;
        }
    }
    cache_len = new_len;
    return cache[n];
}
