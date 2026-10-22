/*
 * A007353  Where the prime race among 5k+1, ..., 5k+4 changes leader.
 * https://oeis.org/A007353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime where the sole leader in {1,2,3,4} (mod 5)
 *            changes
 * Range:     u8 n <= 3, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    uint64_t counts[5] = {0};
    int current_sole_leader = 0;
    idx_t result_count = 0;
    uint64_t candidate = 2;
    
    while (1) {
        if (is_prime(candidate)) {
            int residue = candidate % 5;
            counts[residue]++;
            
            uint64_t max_count = 0;
            int sole_leader = 0;
            int leader_count = 0;
            
            for (int i = 1; i <= 4; i++) {
                if (counts[i] > max_count) {
                    max_count = counts[i];
                    sole_leader = i;
                    leader_count = 1;
                } else if (counts[i] == max_count && max_count > 0) {
                    leader_count++;
                }
            }
            
            if (leader_count > 1) {
                sole_leader = 0;
            }
            
            if (sole_leader != 0 && sole_leader != current_sole_leader) {
                result_count++;
                if (result_count == n) {
                    return (seq_t)candidate;
                }
                current_sole_leader = sole_leader;
            }
        }
        candidate++;
    }
}
