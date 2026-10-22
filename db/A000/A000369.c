/*
 * A000369  Triangle of numbers related to triangle A049213; generalization of
 *          Stirling numbers of second kind A008277, Bessel triangle A001497.
 * https://oeis.org/A000369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n+1, m) = (4*n-m)*a(n, m) + a(n, m-1), with a(n, m) = 0 for n
 *            < m, a(n, 0) = 0, a(1, 1) = 1
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 45, u64 n <= 120,
 *            u128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    // Find row and column from linear index n
    idx_t r = 1;
    idx_t start = 1;
    
    while (start + r <= n) {
        start += r;
        r++;
    }
    
    // m is the position within the row (1-indexed)
    idx_t m = n - start + 1;
    
    // Allocate arrays for two consecutive rows
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    
    // Initialize arrays
    for (idx_t j = 0; j <= r; j++) {
        prev[j] = 0;
        curr[j] = 0;
    }
    
    // Initialize row 1
    prev[1] = 1;
    
    // Build up to row r
    for (idx_t row = 2; row <= r; row++) {
        for (idx_t col = 1; col <= row; col++) {
            if (col == row) {
                curr[col] = prev[col - 1];
            } else {
                seq_t coeff = (seq_t)(4 * (row - 1) - col);
                curr[col] = coeff * prev[col] + prev[col - 1];
            }
        }
        
        // Swap rows
        seq_t *temp = prev;
        prev = curr;
        curr = temp;
    }
    
    seq_t result = prev[m];
    free(prev);
    free(curr);
    
    return result;
}
