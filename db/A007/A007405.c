/*
 * A007405  Dowling numbers: e.g.f.: exp(x + (exp(b*x) - 1)/b) with b=2.
 * https://oeis.org/A007405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = a(n-1) + sum_{k=0}^{n-1} C(n-1,k) * a(k) * 2^(n-1-k),
 *            a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   binom, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pw2.h"

#define MAX_N 20001

static seq_t memo[MAX_N];
static bool computed[MAX_N];
static idx_t max_computed = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (n < MAX_N && computed[n]) {
        return memo[n];
    }
    
    idx_t start = initialized ? (max_computed + 1) : 0;
    initialized = true;
    
    for (idx_t i = start; i <= n && i < MAX_N; i++) {
        if (i == 0) {
            memo[0] = 1;
        } else {
            seq_t result = memo[i - 1];
            
            for (idx_t k = 0; k < i; k++) {
                seq_t term = binom(i - 1, k) * memo[k] * pw2(i - 1 - k);
                result += term;
            }
            
            memo[i] = result;
        }
        
        computed[i] = true;
        max_computed = i;
    }
    
    if (n < MAX_N) {
        return memo[n];
    } else {
        return 0;
    }
}
