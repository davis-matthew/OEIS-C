/*
 * A003713  Expansion of e.g.f. log(1/(1+log(1-x))).
 * https://oeis.org/A003713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 0; a(n) = (n-1)! + Sum_{k=1..n-1} binomial(n-1,k) *
 *            (k-1)! * a(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>
#include "../utils/binom.h"

#define MAX_CACHE 10001

static seq_t cache[MAX_CACHE];
static bool computed[MAX_CACHE];

static void initialize_cache(void) {
    static bool initialized = false;
    if (!initialized) {
        memset(cache, 0, sizeof(cache));
        memset(computed, 0, sizeof(computed));
        cache[0] = 0;
        computed[0] = true;
        initialized = true;
    }
}

seq_t a(idx_t n) {
    initialize_cache();
    
    if (n < MAX_CACHE && computed[n]) {
        return cache[n];
    }
    
    // Compute (n-1)!
    seq_t result = 1;
    for (idx_t i = 2; i < n; i++) {
        result *= (seq_t)i;
    }
    
    // Add the sum: Sum_{k=1..n-1} binomial(n-1,k) * (k-1)! * a(n-k)
    seq_t k_factorial = 1; // (k-1)! for k=1 is 0! = 1
    for (idx_t k = 1; k < n; k++) {
        seq_t binom_coeff = binom((idx_t)(n - 1), k);
        seq_t a_n_minus_k = a(n - k);
        seq_t term = binom_coeff * k_factorial * a_n_minus_k;
        result += term;
        
        // Update k_factorial for the next iteration
        k_factorial *= (seq_t)k;
    }
    
    if (n < MAX_CACHE) {
        cache[n] = result;
        computed[n] = true;
    }
    
    return result;
}
