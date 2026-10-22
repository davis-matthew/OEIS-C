/*
 * A007927  Some nontrivial permutation of digits is a factorial number.
 * https://oeis.org/A007927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   digits_to_num, get_digits, has_factorial_permutation,
 *            is_factorial, next_permutation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/digits_to_num.h"
#include "../utils/get_digits.h"
#include "../utils/has_factorial_permutation.h"
#include "../utils/is_factorial.h"
#include "../utils/next_permutation.h"

/* Sieve: iterate through candidates checking if any digit permutation is a factorial */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;
    
    while (count < n) {
        if (has_factorial_permutation(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
    
    return 0;
}
