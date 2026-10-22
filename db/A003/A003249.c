/*
 * A003249  a(n) = A001950(A003234(n)) + 1.
 * https://oeis.org/A003249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001950(A003234(n)) + 1, where A001950(m) =
 *            floor(m*phi^2) and A003234 lists, by search, the m with
 *            A001950(A003231(m)) = A003231(A001950(m)) + 1, A003231(m) =
 *            floor(m*(5+sqrt(5))/2). All floors are exact: floor(m*phi) = (m
 *            + isqrt(5m^2))/2
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   w_B, w_L, w_U, w_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/w_B.h"
#include "../utils/w_L.h"
#include "../utils/w_U.h"
#include "../utils/w_isqrt.h"

static uint64_t *w_list = NULL;
static idx_t w_cnt = 0, w_cap = 0;
static uint64_t w_next = 1;

seq_t a(idx_t n) {
    while (w_cnt < n) {
        uint64_t m = w_next++;
        if (w_U(w_B(m)) == w_B(w_U(m)) + 1) {
            if (w_cnt == w_cap) {
                idx_t nc = w_cap ? 2 * w_cap : 1024;
                uint64_t *p = (uint64_t *)realloc(w_list, (size_t)nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                w_list = p;
                w_cap = nc;
            }
            w_list[w_cnt++] = m;
        }
    }
    {
        uint64_t c = w_list[n - 1];
        seq_t r = (seq_t)w_U(c);
        r += 1;
        return r;
    }
}
