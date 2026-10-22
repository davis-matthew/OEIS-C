/*
 * A005514  Number of n-bead bracelets (turnover necklaces) with 8 red beads
 *          and n-8 black beads.
 * https://oeis.org/A005514
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((n+4)/32)*s(n,0,8) + ((n-4)/32)*s(n,4,8) + (48*C(n-1,7)
 *            + (n+1)*(n-2)*(n-4)*(n-6))/768 for even n >= 8; a(n) =
 *            (48*C(n-1,7) + (n-1)*(n-3)*(n-5)*(n-7))/768 for odd n >= 8
 * Range:     u8 none, u16 n <= 13, u32 n <= 91, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 8) return 0;
    
    seq_t numerator = 0;
    
    if (n % 2 == 0) {  // even n
        // ((n+4)/32)*s(n,0,8) term: converted to common denominator 768
        if (n % 8 == 0) {
            numerator += ((seq_t)(n + 4)) * 24;
        }
        
        // ((n-4)/32)*s(n,4,8) term: converted to common denominator 768
        if (n % 8 == 4) {
            numerator += ((seq_t)(n - 4)) * 24;
        }
        
        // 48*C(n-1,7) term
        numerator += 48 * binom(n - 1, 7);
        
        // (n+1)*(n-2)*(n-4)*(n-6) term
        numerator += ((seq_t)(n + 1)) * ((seq_t)(n - 2)) * ((seq_t)(n - 4)) * ((seq_t)(n - 6));
    } else {  // odd n
        // 48*C(n-1,7) term
        numerator += 48 * binom(n - 1, 7);
        
        // (n-1)*(n-3)*(n-5)*(n-7) term
        numerator += ((seq_t)(n - 1)) * ((seq_t)(n - 3)) * ((seq_t)(n - 5)) * ((seq_t)(n - 7));
    }
    
    return numerator / 768;
}
