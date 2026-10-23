/*
 * A044277  Numbers k such that the string 2,8 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the base‑9 representation of
 *            k * contains the substring "28" while the base‑9 representation
 *            of k−1 does not
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline bool contains28(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 9);
        if (prev == 8 && d == 2)   /* reverse order when scanning LSB → MSB */
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_k    = 1;      /* last integer examined */

    if (n == 0) return 0;             /* offset is 1, so undefined for n==0 */

    while (cache_len < n) {
        last_k++;                     /* next integer to test */
        seq_t k = (seq_t)last_k;
        bool k_has  = contains28(k);
        bool prev_has = contains28(k - 1);
        if (k_has && !prev_has) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* abort on allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
