/*
 * A003435  Number of directed Hamiltonian circuits on n-octahedron with a
 *          marked starting node.
 * https://oeis.org/A003435
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) =
 *            Sum_{k=0..n}(-1)^k*binomial(n,k)*((2*n)/(2*n-k))*2^k*(2*n-k)!
 *            Using recurrence: (2*n-3)*a(n) = 2*n*(4*n^2-8*n+5)*a(n-1) +
 *            4*(n-1)*n*(2*n-1)*a(n-2) with a(2) = 8, a(3) = 192
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static unsigned char *computed = NULL;
static idx_t memo_capacity = 0;

seq_t a(idx_t n) {
    if (n < 2) return 0;
    
    if (memo == NULL) {
        memo_capacity = (n < 1000) ? 1000 : n + 100;
        memo = (seq_t *)calloc(memo_capacity, sizeof(seq_t));
        computed = (unsigned char *)calloc(memo_capacity, sizeof(unsigned char));
    }
    
    if (n >= memo_capacity) {
        idx_t new_capacity = n + 100;
        memo = (seq_t *)realloc(memo, new_capacity * sizeof(seq_t));
        computed = (unsigned char *)realloc(computed, new_capacity * sizeof(unsigned char));
        for (idx_t i = memo_capacity; i < new_capacity; i++) {
            memo[i] = 0;
            computed[i] = 0;
        }
        memo_capacity = new_capacity;
    }
    
    if (computed[n]) return memo[n];
    
    if (n == 2) {
        computed[2] = 1;
        memo[2] = (seq_t)8;
        return (seq_t)8;
    }
    
    if (n == 3) {
        computed[3] = 1;
        memo[3] = (seq_t)192;
        return (seq_t)192;
    }
    
    seq_t n_seq = (seq_t)n;
    seq_t coeff1 = (seq_t)2 * n_seq * ((seq_t)4 * n_seq * n_seq - (seq_t)8 * n_seq + (seq_t)5);
    seq_t coeff2 = (seq_t)4 * (n_seq - (seq_t)1) * n_seq * ((seq_t)2 * n_seq - (seq_t)1);
    seq_t divisor = (seq_t)2 * n_seq - (seq_t)3;
    
    seq_t a_n_minus_1 = a(n - 1);
    seq_t a_n_minus_2 = a(n - 2);
    
    seq_t numerator = coeff1 * a_n_minus_1 + coeff2 * a_n_minus_2;
    seq_t result = numerator / divisor;
    
    computed[n] = 1;
    memo[n] = result;
    return result;
}
