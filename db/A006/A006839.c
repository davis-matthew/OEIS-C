/*
 * A006839  Minimum of largest partial quotient of continued fraction for k/n,
 *          (k,n) = 1.
 * https://oeis.org/A006839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum of max partial quotient in CF of k/n, k < n,
 *            gcd(k,n)=1, with last quotient = 1
 * Range:     u8 n <= 294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cf_max, gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/cf_max.h"
#include "../utils/gcd_local.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t n_seq = (seq_t)n;
    seq_t min_val = 0;
    bool first = true;
    
    for (seq_t k = 1; k < n_seq; k++) {
        if (gcd_local(k, n_seq) == 1) {
            seq_t max_cf = cf_max(k, n_seq);
            if (first) {
                min_val = max_cf;
                first = false;
            } else if (max_cf < min_val) {
                min_val = max_cf;
            }
        }
    }
    
    return min_val;
}
