/*
 * A003250  The number m such that A001950(m) = A003231(A003234(n)).
 * https://oeis.org/A003250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 5*k - 1 - A003231(k), where k = A003234(n). Search: a
 *            value k counts when A001950(5k-1-A003231(k)) = A003231(k), using
 *            A003231(k) = floor(k*phi) + 2k and A001950(m) = floor(m*phi) + m
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   a3250_fphi, a3250_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/a3250_fphi.h"
#include "../utils/a3250_isqrt.h"

static uint64_t *a3250_cache = NULL;
static idx_t a3250_len = 0, a3250_cap = 0;
static uint64_t a3250_nextk = 1;

seq_t a(idx_t n) {
    while (a3250_len < n) {
        uint64_t k = a3250_nextk++;
        uint64_t B = a3250_fphi(k) + 2 * k;          /* A003231(k) */
        uint64_t m = 5 * k - 1 - B;
        if (m >= 1 && a3250_fphi(m) + m == B) {      /* A001950(m) == B */
            if (a3250_len == a3250_cap) {
                idx_t nc = a3250_cap ? 2 * a3250_cap : 1024;
                a3250_cache = (uint64_t *)realloc(a3250_cache, nc * sizeof(uint64_t));
                a3250_cap = nc;
            }
            a3250_cache[a3250_len++] = m;
        }
    }
    return (seq_t)a3250_cache[n - 1];
}
