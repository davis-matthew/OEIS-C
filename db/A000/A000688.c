/*
 * A000688  Number of Abelian groups of order n; number of factorizations of n
 *          into prime powers.
 * https://oeis.org/A000688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice, mult
 * Formula:   a(n) = Product_{p^e || n} a(p^e), where a(p^k) = number of
 *            partitions of k (A000041)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t partition_cache[64];
static int partition_cache_initialized = 0;

static void init_partition_cache(void) {
    if (partition_cache_initialized) return;
    
    for (int i = 0; i < 64; i++) {
        partition_cache[i] = 0;
    }
    partition_cache[0] = 1;
    
    for (int part = 1; part < 64; part++) {
        for (int i = part; i < 64; i++) {
            partition_cache[i] += partition_cache[i - part];
        }
    }
    
    partition_cache_initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    init_partition_cache();
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        idx_t exponent = 0;
        while (temp % p == 0) {
            exponent++;
            temp /= p;
        }
        
        if (exponent > 0 && exponent < 64) {
            result *= partition_cache[exponent];
        }
    }
    
    return result;
}
