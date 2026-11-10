/*
 * A066948  Triangle of prime signature of A007870(n), read by rows.
 * https://oeis.org/A066948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = k-th largest exponent in the prime factorization of
 *            A007870(r), where row r has pi(r) entries (row 1 is the single
 *            entry 0); the exponent of prime p is Sum_{m>=1} Sum_{p^m | k <=
 *            r} Sum_{j>=1} P(r-j*k), with P the partition numbers
 * Range:     u8 n <= 44, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool tri_is_prime(idx_t x) {
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; d++) if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    idx_t pos = n - 1;
    idx_t r = 2;
    for (;; r++) {
        idx_t len = 0;
        for (idx_t q = 2; q <= r; q++) if (tri_is_prime(q)) len++;
        if (pos <= len) break;
        pos -= len;
    }
    /* partition numbers P(0..r) */
    seq_t *P = mem_new(seq_t, r + 1);
    P[0] = 1;
    for (idx_t k = 1; k <= r; k++) {
        for (idx_t i = k; i <= r; i++) P[i] = P[i] + P[i - k];
    }
    /* occ[k] = total occurrences of part k over partitions of r */
    seq_t *occ = mem_new(seq_t, r + 1);
    for (idx_t k = 1; k <= r; k++) {
        seq_t s = 0;
        for (idx_t j = k; j <= r; j += k) s += P[r - j];
        occ[k] = s;
    }
    seq_t *ex = mem_new(seq_t, r + 1);
    idx_t cnt = 0;
    for (idx_t p = 2; p <= r; p++) {
        if (!tri_is_prime(p)) continue;
        seq_t e = 0;
        for (idx_t k = p; k <= r; k += p) {
            idx_t t = k;
            while (t % p == 0) { e += occ[k]; t /= p; }
        }
        /* insertion into descending order */
        idx_t i = cnt;
        while (i > 0 && ex[i - 1] < e) { ex[i] = ex[i - 1]; i--; }
        ex[i] = e;
        cnt++;
    }
    seq_t res = ex[pos - 1];
    mem_free(ex);
    mem_free(occ);
    mem_free(P);
    return res;
}
