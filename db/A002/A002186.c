/*
 * A002186  Sprague-Grundy values for the game of Kayles (octal games .77 and
 *          .771).
 * https://oeis.org/A002186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Sprague-Grundy values for Kayles (octal .77/.771) a(n) = mex of
 *            Grundy values reachable by removing 1 or 2 adjacent items from a
 *            pile For n >= 71: periodic with period 12
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static seq_t memo[83];
static bool computed = false;

static void compute_base_table(void) {
    memo[0] = 0;
    
    for (int n = 1; n <= 82; n++) {
        seq_t reachable[200];
        int count = 0;
        
        /* Removing 1 item leaves either a(n-1) or splits into a(i) XOR a(n-1-i) */
        reachable[count++] = memo[n - 1];
        for (int i = 1; i < n - 1; i++) {
            reachable[count++] = memo[i] ^ memo[n - 1 - i];
        }
        
        /* Removing 2 items splits into a(k) XOR a(n-2-k) */
        if (n >= 2) {
            for (int k = 0; k <= n - 2; k++) {
                reachable[count++] = memo[k] ^ memo[n - 2 - k];
            }
        }
        
        /* Compute mex (minimum excludant) */
        for (seq_t mex = 0; mex < 256; mex++) {
            int found = 0;
            for (int j = 0; j < count; j++) {
                if (reachable[j] == mex) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                memo[n] = mex;
                break;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!computed) {
        compute_base_table();
        computed = true;
    }
    
    if (n <= 82) {
        return memo[n];
    } else {
        idx_t period_index = 71 + ((n - 71) % 12);
        return memo[period_index];
    }
}
