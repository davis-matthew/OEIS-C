/*
 * A005745  Number of n-covers of an unlabeled 3-set.
 * https://oeis.org/A005745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002727(n) - A002623(n) Computed via generating function:
 *            G.f.: -x*(x^8-x^7-x^6-2*x^5+2*x^4+x^3-3*x^2-2*x-1) /
 *            ((x^3-1)^2*(x^2-1)^2*(x-1)^4)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static const int q[15] = {1, -4, 4, 2, -2, -4, -3, 12, -3, -4, -2, 2, 4, -4, 1};
static const int p[10] = {0, 1, 2, 3, -1, -2, 2, 1, 1, -1};

typedef struct {
    seq_t *cache;
    bool *computed;
    idx_t size;
} Memoization;

static Memoization memo = {NULL, NULL, 0};

static seq_t compute(idx_t n) {
    if (n >= memo.size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        bool *new_computed = (bool *)malloc(new_size * sizeof(bool));
        
        if (memo.cache != NULL) {
            for (idx_t i = 0; i < memo.size; i++) {
                new_cache[i] = memo.cache[i];
                new_computed[i] = memo.computed[i];
            }
            free(memo.cache);
            free(memo.computed);
        }
        
        for (idx_t i = memo.size; i < new_size; i++) {
            new_computed[i] = false;
        }
        
        memo.cache = new_cache;
        memo.computed = new_computed;
        memo.size = new_size;
    }

    if (memo.computed[n]) {
        return memo.cache[n];
    }

    seq_t result = (n < 10) ? (seq_t)p[n] : (seq_t)0;
    
    for (idx_t j = 1; j <= 14 && n - j >= 1; j++) {
        seq_t prev = compute(n - j);
        result = result - prev * (seq_t)q[j];
    }

    memo.computed[n] = true;
    memo.cache[n] = result;
    return result;
}

seq_t a(idx_t n) {
    return compute(n);
}
