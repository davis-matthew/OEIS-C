/*
 * A007729  6th binary partition function.
 * https://oeis.org/A007729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = 2*a(k-1) + a(k); a(2k+1) = 2*a(k) + a(k-1), a(0) = 1,
 *            a(1) = 2
 * Range:     u8 n <= 48, u16 n <= 1642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t cache_indices[500];
static seq_t cache_values[500];
static idx_t cache_count = 0;

static int lookup_cache(idx_t index, seq_t *result) {
    for (idx_t i = 0; i < cache_count; i++) {
        if (cache_indices[i] == index) {
            *result = cache_values[i];
            return 1;
        }
    }
    return 0;
}

static void add_to_cache(idx_t index, seq_t value) {
    if (cache_count < 500) {
        cache_indices[cache_count] = index;
        cache_values[cache_count] = value;
        cache_count++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    seq_t cached;
    if (lookup_cache(n, &cached)) {
        return cached;
    }
    
    seq_t result;
    if (n % 2 == 0) {
        idx_t k = n / 2;
        result = 2 * a(k - 1) + a(k);
    } else {
        idx_t k = (n - 1) / 2;
        result = 2 * a(k) + a(k - 1);
    }
    
    add_to_cache(n, result);
    return result;
}
