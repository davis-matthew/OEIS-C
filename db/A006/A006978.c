/*
 * A006978  Successive states of the Rule 110 cellular automaton defined by
 *          000, 001, 010, 011, ..., 111 -> 0,1,1,1,0,1,1,0 when started with
 *          a single ON cell.
 * https://oeis.org/A006978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Rule 110 cellular automaton (000->0, 001->1, 010->1, 011->1,
 *            100->0, 101->1, 110->1, 111->0) starting with a single ON cell
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static const uint8_t rules[8] = {0, 1, 1, 1, 0, 1, 1, 0};
    
    idx_t size = 2 * n + 10;
    uint8_t *state = (uint8_t *)malloc(size);
    uint8_t *next = (uint8_t *)malloc(size);
    
    for (idx_t i = 0; i < size; i++) {
        state[i] = 0;
        next[i] = 0;
    }
    
    idx_t center = size / 2;
    state[center] = 1;
    
    for (idx_t gen = 0; gen < n - 1; gen++) {
        for (idx_t i = 0; i < size; i++) {
            uint8_t left = (i > 0) ? state[i-1] : 0;
            uint8_t mid = state[i];
            uint8_t right = (i < size - 1) ? state[i+1] : 0;
            uint8_t hood = (left << 2) | (mid << 1) | right;
            next[i] = rules[hood];
        }
        
        uint8_t *tmp = state;
        state = next;
        next = tmp;
    }
    
    idx_t left = 0;
    while (left < size && !state[left]) left++;
    
    if (left >= size) {
        free(state);
        free(next);
        return 0;
    }
    
    idx_t right = size - 1;
    while (right >= left && !state[right]) right--;
    
    seq_t result = 0;
    for (idx_t i = left; i <= right; i++) {
        result = (result << 1) | state[i];
    }
    
    free(state);
    free(next);
    return result;
}
