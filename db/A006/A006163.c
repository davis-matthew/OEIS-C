/*
 * A006163  a(n)=a(a(n-1)-3)+a(n+3-a(n-1)).
 * https://oeis.org/A006163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1) - 3) + a(n + 3 - a(n-1)); initial: a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=4
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static seq_t compute_a(idx_t n);

static seq_t compute_a(idx_t n) {
    if (n >= cache_len) {
        idx_t new_len = (n < 10000) ? 10000 : n + 10000;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        memset(new_cache + cache_len, 0, (new_len + 1 - cache_len) * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len + 1;
    }
    
    if (cache[n] != 0) return cache[n];
    
    seq_t result;
    if (n == 1 || n == 2) result = 1;
    else if (n == 3) result = 2;
    else if (n == 4) result = 3;
    else if (n == 5) result = 4;
    else {
        seq_t an_minus_1 = compute_a(n - 1);
        idx_t idx1 = (idx_t)an_minus_1 - 3;
        idx_t idx2 = n + 3 - (idx_t)an_minus_1;
        result = compute_a(idx1) + compute_a(idx2);
    }
    
    cache[n] = result;
    return result;
}

seq_t a(idx_t n) {
    return compute_a(n);
}
