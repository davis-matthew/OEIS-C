/*
 * A005116  Number of protruded partitions of n with largest part at most 10.
 * https://oeis.org/A005116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1-x)^10/Product(1-x-x^i+x^(1+2*i), i=1..10)-1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    int target = (int)n;
    if (target < 1) return 0;
    
    long long *num = (long long *)calloc(target + 1, sizeof(long long));
    long long *den = (long long *)calloc(target + 1, sizeof(long long));
    long long *result = (long long *)calloc(target + 1, sizeof(long long));
    
    if (!num || !den || !result) {
        free(num);
        free(den);
        free(result);
        return 0;
    }
    
    // Compute numerator: (1-x)^10 = sum C(10,k)(-1)^k x^k
    num[0] = 1;
    long long binom = 1;
    for (int k = 1; k <= 10 && k <= target; k++) {
        binom = binom * (10 - k + 1) / k;
        num[k] = (k % 2 == 0) ? binom : -binom;
    }
    
    // Compute denominator: P(x) = Product(1 - x - x^i + x^(2i+1), i=1..10)
    den[0] = 1;
    for (int i = 1; i <= 10; i++) {
        long long *temp = (long long *)calloc(target + 1, sizeof(long long));
        if (!temp) {
            free(num);
            free(den);
            free(result);
            return 0;
        }
        
        // Multiply current den by (1 - x - x^i + x^(2i+1))
        for (int j = 0; j <= target; j++) {
            temp[j] = den[j];
            if (j >= 1) temp[j] -= den[j-1];
            if (j >= i) temp[j] -= den[j-i];
            if (j >= 2*i + 1) temp[j] += den[j - 2*i - 1];
        }
        
        memcpy(den, temp, (target + 1) * sizeof(long long));
        free(temp);
    }
    
    // Polynomial division: H(x) * P(x) = (1-x)^10
    // H(x) = (1-x)^10 / P(x), and G(x) = H(x) - 1
    // a(n) = H_n for n >= 1
    for (int k = 0; k <= target; k++) {
        result[k] = num[k];
        for (int i = 1; i <= k; i++) {
            result[k] -= result[k-i] * den[i];
        }
    }
    
    seq_t answer = (seq_t)result[target];
    
    free(num);
    free(den);
    free(result);
    
    return answer;
}
