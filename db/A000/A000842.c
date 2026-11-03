/*
 * A000842  Number of n-input 3-output switching networks under action of
 *          symmetric group S(n) on the inputs and complementing group C(3,2)
 *          on the outputs.
 * https://oeis.org/A000842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(8*n!)) * Σ_{σ ∈ S_n} 8^{c(σ)}, where c(σ) is the
 *            number of cycles of σ acting on {0,1}^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned long long count_cycles_for_perm(const idx_t *perm, idx_t n) {
    if (n > 20) return 0;
    
    unsigned long long N = 1ULL << n;
    char *visited = (char *)calloc(N, 1);
    
    unsigned long long num_cycles = 0;
    
    for (unsigned long long S = 0; S < N; S++) {
        if (!visited[S]) {
            unsigned long long current = S;
            do {
                visited[current] = 1;
                
                unsigned long long next = 0;
                for (idx_t i = 0; i < n; i++) {
                    if ((current >> i) & 1ULL) {
                        next |= (1ULL << perm[i]);
                    }
                }
                current = next;
            } while (current != S);
            
            num_cycles++;
        }
    }
    
    free(visited);
    return num_cycles;
}

static seq_t factorial_seq(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}

static seq_t pow_eight(unsigned long long e) {
    seq_t result = 1;
    for (unsigned long long i = 0; i < e; i++) {
        result *= 8;
    }
    return result;
}

static void gen_perms_recursive(idx_t *perm, idx_t n, idx_t pos, seq_t *sum) {
    if (pos == n) {
        unsigned long long c = count_cycles_for_perm(perm, n);
        *sum += pow_eight(c);
        return;
    }
    
    for (idx_t i = pos; i < n; i++) {
        idx_t temp = perm[pos];
        perm[pos] = perm[i];
        perm[i] = temp;
        
        gen_perms_recursive(perm, n, pos + 1, sum);
        
        perm[i] = perm[pos];
        perm[pos] = temp;
    }
}

seq_t a(idx_t n) {
    if (n == 0 || n > 20) return 0;
    
    idx_t *perm = (idx_t *)malloc(n * sizeof(idx_t));
    for (idx_t i = 0; i < n; i++) {
        perm[i] = i;
    }
    
    seq_t sum_value = 0;
    gen_perms_recursive(perm, n, 0, &sum_value);
    
    free(perm);
    
    seq_t n_fact = factorial_seq(n);
    seq_t denom = 8 * n_fact;
    
    return sum_value / denom;
}
