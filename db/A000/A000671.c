/*
 * A000671  Number of boron trees with n nodes, i.e. n-node rooted trees with
 *          degree <= 3 at root and out-degree <= 2 elsewhere.
 * https://oeis.org/A000671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) counts rooted trees with n nodes where the root has
 *            out-degree <= 3 * and all other nodes have out-degree <= 2. * *
 *            Computed via dynamic programming: first compute trees2[i] =
 *            number of trees * with i nodes where every node (including root)
 *            has out-degree <= 2, then * enumerate multisets of...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 55, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t *trees2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    trees2[1] = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        trees2[i] += trees2[i - 1];
        
        for (idx_t j = 1; j * 2 <= i - 1; j++) {
            idx_t k = i - 1 - j;
            if (j < k) {
                trees2[i] += trees2[j] * trees2[k];
            } else if (j == k) {
                seq_t tj = trees2[j];
                trees2[i] += tj * (tj + 1) / 2;
            }
        }
    }
    
    seq_t result = 0;
    
    if (n >= 2) {
        result += trees2[n - 1];
    }
    
    for (idx_t j = 1; j * 2 <= n - 1; j++) {
        idx_t k = n - 1 - j;
        if (j < k) {
            result += trees2[j] * trees2[k];
        } else if (j == k) {
            seq_t tj = trees2[j];
            result += tj * (tj + 1) / 2;
        }
    }
    
    for (idx_t j = 1; j * 3 <= n - 1; j++) {
        for (idx_t k = j; j + k * 2 <= n - 1; k++) {
            idx_t l = n - 1 - j - k;
            if (l >= k) {
                seq_t tj = trees2[j];
                seq_t tk = trees2[k];
                seq_t tl = trees2[l];
                
                if (j == k && k == l) {
                    result += tj * (tj + 1) * (tj + 2) / 6;
                } else if (j == k && k < l) {
                    result += tj * (tj + 1) / 2 * tl;
                } else if (j < k && k == l) {
                    result += tj * tk * (tk + 1) / 2;
                } else if (j < k && k < l) {
                    result += tj * tk * tl;
                }
            }
        }
    }
    
    free(trees2);
    return result;
}
