/*
 * A089967  Numbers k such that Omega(k) = gpf(k) mod lpf(n), where
 *          Omega=A001222, gpf=A006530 and lpf=A020639.
 * https://oeis.org/A089967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that Omega(k) = gpf(k)
 *            mod lpf(k)
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   bigomega, gpf, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"
#include "../utils/lpf.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but return 0 for safety */
    idx_t count = 0;
    uint64_t k = 1;
    while (1) {
        seq_t omega = bigomega(k);
        uint64_t g = gpf(k);
        uint64_t l = lpf(k);
        /* avoid division by zero if l==0; treat as mismatch */
        if (l != 0 && (g % l) == (uint64_t)omega) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
}
