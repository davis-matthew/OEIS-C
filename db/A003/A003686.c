/*
 * A003686  Number of genealogical 1-2 rooted trees of height n.
 * https://oeis.org/A003686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = a(n) + product(a(1)...a(n-1)) for n >= 2, with a(1)=1,
 *            a(2)=2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   grow_arr, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/zalloc_arr.h"
#include "../utils/grow_arr.h"


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static seq_t *products = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;
    
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_size = 100;
        cache = zalloc_arr(cache_size);
        products = zalloc_arr(cache_size);
        cache[0] = 1;
        cache[1] = 2;
        products[0] = 1;
        products[1] = 2;
        computed_up_to = 2;
    }
    
    while (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        cache = grow_arr(cache, cache_size, new_size);
        products = grow_arr(products, cache_size, new_size);
        cache_size = new_size;
    }
    
    if (n <= computed_up_to) {
        return cache[n - 1];
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t product_term = (i >= 3) ? products[i - 3] : 1;
        cache[i - 1] = cache[i - 2] + product_term;
        products[i - 1] = products[i - 2] * cache[i - 1];
    }
    
    computed_up_to = n;
    return cache[n - 1];
}
