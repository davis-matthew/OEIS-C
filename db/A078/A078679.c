/*
 * A078679  Number of Grand Motzkin paths of length n with no zigzags, that is
 *          with no factors UDU and DUD.
 * https://oeis.org/A078679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies 0 = (n+6)*a(n+6) - (4n+21)*a(n+5) +
 *            (4n+15)*a(n+4) - (2n+3)*a(n+3) + a(n+2) - a(n+1) + (n+1)*a(n)
 *            with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=17,
 *            a(5)=43
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;
static idx_t seq_cache_cap = 0;

static void ensure_cap(idx_t needed)
{
    if (seq_cache_cap >= needed) return;
    idx_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 32;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);                   /* out of memory */
    seq_cache = new_ptr;
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Constant initial values */
    const seq_t init_vals[6] = { (seq_t)1, (seq_t)1, (seq_t)3,
                                 (seq_t)7, (seq_t)17, (seq_t)43 };
    if (n < 6) return init_vals[n];

    /* Ensure cache can hold indices 0…5 */
    ensure_cap(6);
    if (seq_cache_len < 6) {
        for (idx_t i = 0; i < 6; ++i) seq_cache[i] = init_vals[i];
        seq_cache_len = 6;
    }

    /* Extend cache to n */
    ensure_cap(n + 1);
    for (idx_t k = seq_cache_len; k <= n; ++k) {
        /* k is the current index to compute a(k) */
        idx_t kk = k;                               /* rename for clarity */
        seq_t v1 = (seq_t)(4 * (kk - 6) + 21) * seq_cache[kk - 1];
        seq_t v2 = (seq_t)(4 * (kk - 6) + 15) * seq_cache[kk - 2];
        seq_t v3 = (seq_t)(2 * (kk - 6) + 3) * seq_cache[kk - 3];
        seq_t v4 = seq_cache[kk - 4];
        seq_t v5 = seq_cache[kk - 5];
        seq_t v6 = (seq_t)(kk - 5) * seq_cache[kk - 6];
        seq_t num = v1 - v2 + v3 - v4 + v5 - v6;
        seq_cache[kk] = num / (seq_t)kk;
    }
    seq_cache_len = n + 1;
    return seq_cache[n];
}
