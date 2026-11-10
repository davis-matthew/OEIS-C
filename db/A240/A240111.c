/*
 * A240111  Numbers for which the value of the Dedekind psi function (A001615)
 *          are less than the value of the infinitary Dedekind psi function
 *          (A049417).
 * https://oeis.org/A240111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A001615(k) < A049417(k), i.e. Product_{p|k}
 *            (1+1/p) < Product_{q in Q_k} (1+1/q); found by scanning k upward
 *            (search)
 * Range:     u8 n <= 28, u16 n <= 6993, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9744 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A240111_cache = NULL;
static size_t A240111_cnt = 0, A240111_cap = 0;
static uint64_t A240111_next = 1;

static bool A240111_test(uint64_t k) {
    uint64_t psi = 1, isig = 1, m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t c = p + 1;
            for (uint64_t i = 1; i < e; i++) c *= p;
            psi *= c;
            uint64_t pw = p;
            uint64_t ee = e;
            while (ee) {
                if (ee & 1) isig *= (pw + 1);
                ee >>= 1;
                if (ee) pw *= pw;
            }
        }
    }
    if (m > 1) { psi *= (m + 1); isig *= (m + 1); }
    return psi < isig;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t need = (size_t)n;
    while (A240111_cnt < need) {
        if (A240111_test(A240111_next)) {
            if (A240111_cnt == A240111_cap) {
                A240111_cap = A240111_cap ? A240111_cap * 2 : 1024;
                A240111_cache = (uint64_t *)realloc(A240111_cache, A240111_cap * sizeof(uint64_t));
            }
            A240111_cache[A240111_cnt++] = A240111_next;
        }
        A240111_next++;
    }
    return (seq_t)A240111_cache[need - 1];
}
