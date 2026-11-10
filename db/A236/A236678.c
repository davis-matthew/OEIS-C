/*
 * A236678  Partial sums of the characteristic function of A079599.
 * https://oeis.org/A236678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n-1) + A236677(n), where A236677(0) = 1 and
 *            A236677(n) = (1 - A236677(floor(log_2(n)))) * A236677(n -
 *            2^floor(log_2(n)))
 * Range:     u8 n <= 2809, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 90 terms through n = 7974 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static unsigned char *chi_c = NULL;
static seq_t *sum_c = NULL;
static size_t len_c = 0;
static size_t cap_c = 0;

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > len_c) {
        if (need > cap_c) {
            size_t nc = cap_c ? cap_c : 64;
            while (nc < need) nc <<= 1;
            unsigned char *t = (unsigned char *)realloc(chi_c, nc);
            if (!t) abort();
            chi_c = t;
            sum_c = mem_resize(seq_t, sum_c, nc);
            cap_c = nc;
        }
        for (size_t i = len_c; i < need; ++i) {
            unsigned char c;
            if (i == 0) {
                c = 1;
            } else {
                size_t lg = 0;
                while (((size_t)2 << lg) <= i) ++lg;
                size_t msb = (size_t)1 << lg;
                c = (chi_c[lg] == 0) ? chi_c[i - msb] : 0;
            }
            chi_c[i] = c;
            if (i == 0) sum_c[0] = (seq_t)c;
            else { sum_c[i] = sum_c[i - 1]; if (c) sum_c[i] += 1; }
        }
        len_c = need;
    }
    return sum_c[(size_t)n];
}
