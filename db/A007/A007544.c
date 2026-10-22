/*
 * A007544  Frequency of n-th largest distance in N times N times N grid, N >
 *          n.
 * https://oeis.org/A007544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For N = n+1, count all pairwise distances in an N×N×N grid, find
 *            the n-th largest distance value by frequency, return its
 *            frequency
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t N = n + 1;
    
    // Maximum possible squared distance is 3*(N-1)²
    idx_t max_dist_sq = 3 * (N - 1) * (N - 1);
    seq_t *freq = (seq_t *)calloc((size_t)(max_dist_sq + 1), sizeof(seq_t));
    if (freq == NULL) return 0;
    
    // For each possible absolute displacement vector
    for (idx_t adx = 0; adx < N; adx++) {
        for (idx_t ady = 0; ady < N; ady++) {
            for (idx_t adz = 0; adz < N; adz++) {
                if (adx == 0 && ady == 0 && adz == 0) continue;
                
                // Count ordered pairs with this absolute displacement
                // If adx == 0, points can align in x: N choices
                // If adx > 0, points can move ±adx in x: 2*(N-adx) choices
                seq_t count_x = (adx == 0) ? (seq_t)N : (seq_t)(2 * (N - adx));
                seq_t count_y = (ady == 0) ? (seq_t)N : (seq_t)(2 * (N - ady));
                seq_t count_z = (adz == 0) ? (seq_t)N : (seq_t)(2 * (N - adz));
                
                seq_t pair_count = count_x * count_y * count_z;
                
                idx_t dist_sq = adx*adx + ady*ady + adz*adz;
                freq[dist_sq] += pair_count;
            }
        }
    }
    
    // Divide by 2: we counted each unordered pair twice
    for (idx_t d = 1; d <= max_dist_sq; d++) {
        freq[d] /= 2;
    }
    
    // Find the n-th largest distance value
    seq_t result = 0;
    idx_t count = 0;
    for (idx_t d = max_dist_sq; d >= 1; d--) {
        if (freq[d] > 0) {
            count++;
            if (count == n) {
                result = freq[d];
                break;
            }
        }
    }
    
    free(freq);
    return result;
}
