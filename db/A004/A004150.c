/*
 * A004150  Euler numbers written backwards.
 * https://oeis.org/A004150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000111(n)), where A000111(n) are Euler numbers
 *            computed via: a(n) = Sum_{k=0..floor((n-1)/2)} C(n-1, 2k) *
 *            a(2k) * a(n-1-2k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include <stdlib.h>

static seq_t reverse_digits(seq_t num) {
    seq_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    return reversed;
}

seq_t a(idx_t n) {
    static seq_t *euler = NULL;
    static idx_t euler_len = 0;
    
    if (n >= euler_len) {
        idx_t new_len = 1;
        while (new_len <= n) new_len *= 2;
        
        seq_t *new_euler = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (euler != NULL) {
            for (idx_t i = 0; i < euler_len; i++) {
                new_euler[i] = euler[i];
            }
            free(euler);
        }
        euler = new_euler;
        
        idx_t start = euler_len;
        if (start == 0) {
            euler[0] = 1;
            start = 1;
            euler_len = 1;
        }
        
        for (idx_t i = start; i <= n; i++) {
            seq_t sum = 0;
            for (idx_t k = 0; k * 2 <= i - 1; k++) {
                seq_t binom_coeff = binom(i - 1, 2 * k);
                seq_t euler_product = euler[2 * k] * euler[i - 1 - 2 * k];
                sum += binom_coeff * euler_product;
            }
            euler[i] = sum;
        }
        euler_len = n + 1;
    }
    
    return reverse_digits(euler[n]);
}
