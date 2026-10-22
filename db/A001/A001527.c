/*
 * A001527  a(n) = 2 * Sum_{i=0..n} C(2^n-1, i).
 * https://oeis.org/A001527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Sum_{i=0..n} C(2^n-1, i), with C(M,j+1) =
 *            C(M,j)*(M-j)/(j+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   a001527_big, a001527_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001527_big.h"
#include "../utils/a001527_inv.h"

seq_t a(idx_t n) {
    if (a001527_big()) {
        seq_t M = ((seq_t)1 << n) - 1;
        seq_t term = 1, sum = 1;
        idx_t j;
        for (j = 0; j < n; j++) {
            term = term * (M - (seq_t)j) / ((seq_t)j + 1);
            sum += term;
        }
        return sum * 2;
    } else {
        /* Work mod 2^128; C(2^n-1, i) is odd, and the 2-adic valuations of
           (2^n-1-j) and (j+1) coincide, so only odd parts are needed. */
        unsigned __int128 term = 1, sum = 1;
        idx_t j;
        for (j = 0; j < n; j++) {
            uint64_t q = (uint64_t)j + 1;
            idx_t v = 0;
            while ((q & 1) == 0) { q >>= 1; v++; }
            idx_t e = n - v;
            unsigned __int128 pw = (e >= 128) ? 0 : ((unsigned __int128)1 << e);
            unsigned __int128 num = pw - (unsigned __int128)q;
            term = term * num * a001527_inv((unsigned __int128)q);
            sum += term;
        }
        sum = sum * 2;
        uint64_t hi = (uint64_t)(sum >> 64);
        uint64_t lo = (uint64_t)sum;
        seq_t r = (seq_t)hi;
        int k;
        for (k = 0; k < 64; k++) r = r + r;
        r = r + (seq_t)lo;
        return r;
    }
}
