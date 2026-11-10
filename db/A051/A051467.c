/*
 * A051467  (Terms in A029640)/2.
 * https://oeis.org/A051467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th term of A029640)/2; rows of the (1,2)-Pascal
 *            triangle A029635 (row r: T(r,0)=1, T(r,r)=2,
 *            T(r,j)=T(r-1,j-1)+T(r-1,j)) are scanned left to right, and the
 *            even entries, halved, are listed in order (offset 0)
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t cap = 4;
    seq_t *prev = mem_new(seq_t, cap);
    seq_t *cur = mem_new(seq_t, cap);
    prev[0] = (seq_t)1;           /* row 0 */
    idx_t count = 0;
    for (size_t r = 1;; ++r) {
        if (r + 1 > cap) {
            size_t ncap = cap * 2;
            while (ncap < r + 1) ncap *= 2;
            seq_t *np = mem_new(seq_t, ncap);
            seq_t *nc = mem_new(seq_t, ncap);
            for (size_t j = 0; j < r; ++j) np[j] = prev[j];
            mem_free(prev);
            mem_free(cur);
            prev = np;
            cur = nc;
            cap = ncap;
        }
        cur[0] = (seq_t)1;
        for (size_t j = 1; j < r; ++j)
            cur[j] = prev[j - 1] + prev[j];
        cur[r] = (seq_t)2;
        for (size_t j = 1; j <= r; ++j) {
            if ((cur[j] & (seq_t)1) == 0) {
                if (count == n) {
                    seq_t ans = cur[j] / 2;
                    mem_free(prev);
                    mem_free(cur);
                    return ans;
                }
                ++count;
            }
        }
        seq_t *t = prev;
        prev = cur;
        cur = t;
    }
}
