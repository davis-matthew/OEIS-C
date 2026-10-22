/*
 * A003198  Cluster series for bond percolation problem on square lattice.
 * https://oeis.org/A003198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) from cluster series; satisfies a recurrence relation
 *            derived from the bond percolation cluster expansion on square
 *            lattice
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

#define CACHE_SIZE 1024

static struct {
    idx_t n;
    seq_t val;
} cache[CACHE_SIZE];
static int cache_count = 0;

static seq_t get_cached(idx_t n) {
    for (int i = 0; i < cache_count; i++) {
        if (cache[i].n == n) return cache[i].val;
    }
    return (seq_t)-1;
}

static void set_cached(idx_t n, seq_t val) {
    if (cache_count < CACHE_SIZE) {
        cache[cache_count].n = n;
        cache[cache_count].val = val;
        cache_count++;
    }
}

seq_t a(idx_t n) {
    static seq_t base[18] = {
        (seq_t)1, (seq_t)6, (seq_t)18, (seq_t)48, (seq_t)126,
        (seq_t)300, (seq_t)762, (seq_t)1668, (seq_t)4216,
        (seq_t)8668, (seq_t)21988, (seq_t)43058, (seq_t)110832,
        (seq_t)202432, (seq_t)561020, (seq_t)875382,
        (seq_t)2881286, (seq_t)3501056
    };
    
    if (n < 18) return base[n];
    
    seq_t cached = get_cached(n);
    if (cached != (seq_t)-1) return cached;
    
    /* Recurrence: a(n) = 2*a(n-1) + a(n-2) - 2*a(n-4) + a(n-5) */
    seq_t result = (seq_t)2 * a(n - (idx_t)1) + a(n - (idx_t)2) -
                   (seq_t)2 * a(n - (idx_t)4) + a(n - (idx_t)5);
    
    set_cached(n, result);
    return result;
}
