/*
 * A003620  Number of iterations until 3n reaches 153 under x goes to sum of
 *          cubes of digits map.
 * https://oeis.org/A003620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations until 3*n reaches 153 under sum of
 *            cubes of digits map
 * Range:     u8 none, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   sum_of_cubes_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_of_cubes_of_digits.h"

seq_t a(idx_t n) {
    seq_t current = (seq_t)n * 3;
    seq_t iterations = 0;
    
    while (current != 153) {
        current = sum_of_cubes_of_digits(current);
        iterations++;
    }
    
    return iterations;
}
