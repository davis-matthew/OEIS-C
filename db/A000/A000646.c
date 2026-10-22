/*
 * A000646  Number of alkyls Y^{II} C_n H_{2n+2} with n carbon atoms.
 * https://oeis.org/A000646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n} A000598(k) * A000598(n-k), where A000598(n)
 *            is the number of rooted unlabeled trees with n nodes, each node
 *            having at most 3 children
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 10001

static seq_t cache_598[MAX_N];
static bool computed_598[MAX_N];

seq_t a000598(idx_t n);

seq_t a000598(idx_t n) {
    if (n >= MAX_N) return 0;
    if (n == 0) return 1;
    if (n == 1) return 1;
    
    if (computed_598[n]) return cache_598[n];
    
    seq_t result = 0;
    
    for (idx_t p1 = 0; p1 * 3 < n; p1++) {
        idx_t remaining = n - 1 - p1;
        for (idx_t p2 = p1; p2 * 2 <= remaining; p2++) {
            idx_t p3 = remaining - p2;
            
            seq_t a1 = a000598(p1);
            seq_t a2 = a000598(p2);
            seq_t a3 = a000598(p3);
            
            seq_t term;
            if (p1 == p2 && p2 == p3) {
                term = a1 * (a1 + 1) * (a1 + 2) / 6;
            } else if (p1 == p2) {
                term = a1 * (a1 + 1) / 2 * a3;
            } else if (p2 == p3) {
                term = a1 * a2 * (a2 + 1) / 2;
            } else {
                term = a1 * a2 * a3;
            }
            
            result += term;
        }
    }
    
    computed_598[n] = true;
    return cache_598[n] = result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        result += a000598(k) * a000598(n - k);
    }
    return result;
}
