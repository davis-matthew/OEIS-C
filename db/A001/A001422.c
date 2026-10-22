/*
 * A001422  Numbers which are not the sum of distinct squares.
 * https://oeis.org/A001422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers which cannot be expressed as sums of distinct squares.
 *            Use DP to compute all representable numbers: for each perfect
 *            square k^2, mark all i as representable if i - k^2 was already
 *            representable
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>


#define MAX_CHECK 1000

static bool representable[MAX_CHECK + 1];
static int precomputed = 0;

static void precompute(void) {
    if (precomputed) return;
    
    memset(representable, 0, sizeof(representable));
    representable[0] = true;
    
    // For each perfect square k^2
    for (uint64_t k = 1; k * k <= MAX_CHECK; k++) {
        uint64_t sq = k * k;
        
        // Mark all sums (backwards to use each square at most once)
        for (int i = MAX_CHECK; i >= (int)sq; i--) {
            if (representable[i - sq]) {
                representable[i] = true;
            }
        }
    }
    
    precomputed = 1;
}

seq_t a(idx_t n) {
    precompute();
    
    seq_t count = 0;
    seq_t num = 1;
    
    while (num <= MAX_CHECK) {
        if (!representable[(int)num]) {
            count++;
            if (count == (seq_t)n) return num;
        }
        num++;
    }
    
    return (seq_t)MAX_CHECK;
}
