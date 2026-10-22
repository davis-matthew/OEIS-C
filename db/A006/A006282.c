/*
 * A006282  Sorting numbers: number of comparisons in Batcher's parallel sort.
 * https://oeis.org/A006282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 0, a(n) = a(ceil(n/2)) + a(floor(n/2)) + C(ceil(n/2),
 *            floor(n/2)) for n > 1, where C(m,n) = m*n if m*n <= 1, else
 *            C(ceil(m/2), ceil(n/2)) + C(floor(m/2), floor(n/2)) +
 *            floor((m+n-1)/2)
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *memo_a = NULL;
static char *computed_a = NULL;
static idx_t memo_a_len = 0;

typedef struct {
    idx_t m, n;
    seq_t value;
} CacheEntry;

static CacheEntry *cache_c = NULL;
static idx_t cache_c_len = 0;
static idx_t cache_c_cap = 0;

static seq_t C(idx_t m, idx_t n);

static void ensure_memo_a(idx_t n) {
    if (n >= memo_a_len) {
        idx_t new_len = n + 100;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        char *new_computed = (char *)malloc(new_len * sizeof(char));
        
        if (memo_a) {
            memcpy(new_memo, memo_a, memo_a_len * sizeof(seq_t));
            memcpy(new_computed, computed_a, memo_a_len * sizeof(char));
            free(memo_a);
            free(computed_a);
        }
        
        memo_a = new_memo;
        computed_a = new_computed;
        memset(computed_a + memo_a_len, 0, (new_len - memo_a_len) * sizeof(char));
        memo_a_len = new_len;
    }
}

static seq_t C(idx_t m, idx_t n) {
    for (idx_t i = 0; i < cache_c_len; i++) {
        if (cache_c[i].m == m && cache_c[i].n == n) {
            return cache_c[i].value;
        }
    }
    
    seq_t result;
    
    if (m == 0 || n == 0) {
        result = 0;
    } else if (m == 1 && n == 1) {
        result = 1;
    } else {
        idx_t m_ceil = (m + 1) / 2;
        idx_t m_floor = m / 2;
        idx_t n_ceil = (n + 1) / 2;
        idx_t n_floor = n / 2;
        
        seq_t c1 = C(m_ceil, n_ceil);
        seq_t c2 = C(m_floor, n_floor);
        seq_t c3 = (seq_t)((m + n - 1) / 2);
        
        result = c1 + c2 + c3;
    }
    
    if (cache_c_len >= cache_c_cap) {
        idx_t new_cap = cache_c_cap ? cache_c_cap * 2 : 100;
        CacheEntry *new_cache = (CacheEntry *)malloc(new_cap * sizeof(CacheEntry));
        if (cache_c) {
            memcpy(new_cache, cache_c, cache_c_len * sizeof(CacheEntry));
            free(cache_c);
        }
        cache_c = new_cache;
        cache_c_cap = new_cap;
    }
    
    cache_c[cache_c_len].m = m;
    cache_c[cache_c_len].n = n;
    cache_c[cache_c_len].value = result;
    cache_c_len++;
    
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    ensure_memo_a(n);
    
    if (computed_a[n]) {
        return memo_a[n];
    }
    
    idx_t n_ceil = (n + 1) / 2;
    idx_t n_floor = n / 2;
    
    seq_t a_ceil = a(n_ceil);
    seq_t a_floor = a(n_floor);
    seq_t c_val = C(n_ceil, n_floor);
    
    seq_t result = a_ceil + a_floor + c_val;
    memo_a[n] = result;
    computed_a[n] = 1;
    
    return result;
}
