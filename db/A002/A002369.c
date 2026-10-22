/*
 * A002369  Number of ways of folding a strip of n rectangular stamps.
 * https://oeis.org/A002369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(n) counts foldings of a strip of n stamps using dynamic
 *            programming. A folding is represented by tracking which stamps
 *            are bonded together and their relative orientations, enumerating
 *            all distinct folded configurations
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   compute_foldings
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/compute_foldings.h"

#define MAX_N 20
static seq_t memo[MAX_N + 1];
static int computed[MAX_N + 1];

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n <= MAX_N && computed[n]) return memo[n];
    
    seq_t val = compute_foldings(n);
    
    if (n <= MAX_N) {
        computed[n] = 1;
        memo[n] = val;
    }
    
    return val;
}
