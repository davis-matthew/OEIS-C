/*
 * A006865  Number of Hamiltonian cycles in P_5 X P_{2n}.
 * https://oeis.org/A006865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 2*a(n-3), a(1) = 1, a(2) = 14, a(3) = 154
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1 || n >= CACHE_SIZE) return 0;
    
    // Initialize base cases on first call
    if (computed_up_to == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)154;
        computed_up_to = 3;
    }
    
    // Compute values up to n if not already done
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            cache[i] = (seq_t)11 * cache[i - 1] + (seq_t)2 * cache[i - 3];
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
