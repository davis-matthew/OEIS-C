/*
 * A004208  a(n) = n * (2*n - 1)!! - Sum_{k=0..n-1} a(k) * (2*n - 2*k - 1)!!.
 * https://oeis.org/A004208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (2*n - 1)!! - Sum_{k=0..n-1} a(k) * (2*n - 2*k - 1)!!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;
static char *a_computed = NULL;
static idx_t a_cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (a_cache_size <= n) {
        idx_t new_size = (n + 1) * 2;
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        a_computed = (char *)realloc(a_computed, new_size);
        memset(a_computed + a_cache_size, 0, new_size - a_cache_size);
        a_cache_size = new_size;
    }
    
    if (a_computed[n]) return a_cache[n];
    
    seq_t df_n = 1;
    for (idx_t i = 1; i <= n; i++) {
        df_n *= (2 * i - 1);
    }
    
    seq_t result = (seq_t)n * df_n;
    
    for (idx_t k = 0; k < n; k++) {
        seq_t ak = a(k);
        seq_t df = 1;
        for (idx_t i = 1; i <= n - k; i++) {
            df *= (2 * i - 1);
        }
        result -= ak * df;
    }
    
    a_cache[n] = result;
    a_computed[n] = 1;
    return result;
}
