/*
 * A170972  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 9.
 * https://oeis.org/A170972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of subsets of {1,5,9,13,17,21,25,29,33} whose
 *            elements sum to n
 * Range:     u8 n <= 153, u16 n <= 153, u32 n <= 153, u64 n <= 153,
 *            u128 n <= 153, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_SUM 153

seq_t a(idx_t n)
{
    static seq_t coeff[MAX_SUM + 1];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t mask = 0; mask < (1ULL << 9); ++mask) {
            idx_t sum = 0;
            if (mask & 1ULL)    sum += 1;
            if (mask & 2ULL)    sum += 5;
            if (mask & 4ULL)    sum += 9;
            if (mask & 8ULL)    sum += 13;
            if (mask & 16ULL)   sum += 17;
            if (mask & 32ULL)   sum += 21;
            if (mask & 64ULL)   sum += 25;
            if (mask & 128ULL)  sum += 29;
            if (mask & 256ULL)  sum += 33;
            coeff[(size_t)sum] += (seq_t)1;
        }
        initialized = true;
    }

    if (n > MAX_SUM)
        return (seq_t)0;
    return coeff[(size_t)n];
}
