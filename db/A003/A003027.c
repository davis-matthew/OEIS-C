/*
 * A003027  Number of weakly connected digraphs with n labeled nodes.
 * https://oeis.org/A003027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A053763(n) - (1/n) * Sum_{k=1..n-1}
 *            k*C(n,k)*a(k)*A053763(n-k) where A053763(n) = 2^(n(n-1)) is the
 *            total number of digraphs with n labeled nodes
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/pow2.h"

static seq_t cache[10001];
static char cached[10001];

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result = result * ((seq_t)n - (seq_t)i) / ((seq_t)i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (n <= 10000 && cached[n]) {
        return cache[n];
    }
    
    idx_t exp = n * (n - 1);
    seq_t total = pow2(exp);
    
    seq_t sum = 0;
    for (idx_t k = 1; k < n; k++) {
        idx_t nk = n - k;
        idx_t exp_nk = nk * (nk - 1);
        seq_t term = (seq_t)k * binom(n, k) * a(k) * pow2(exp_nk);
        sum += term;
    }
    
    seq_t result = total - sum / (seq_t)n;
    
    if (n <= 10000) {
        cache[n] = result;
        cached[n] = 1;
    }
    
    return result;
}
