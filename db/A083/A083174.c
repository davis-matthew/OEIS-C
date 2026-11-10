/*
 * A083174  Leading diagonal of A083173.
 * https://oeis.org/A083174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * k, where k = n if n is odd and k = 3n/2 if n
 *            is even (last entry of row n of A083173)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A083174_pr = NULL;
static size_t A083174_len = 0, A083174_cap = 0;

static uint64_t A083174_prime(idx_t n)
{
    size_t need = (size_t)n;
    if (need > A083174_len) {
        if (need > A083174_cap) {
            size_t nc = A083174_cap ? A083174_cap : 64;
            while (nc < need) nc *= 2;
            uint64_t *t = (uint64_t *)realloc(A083174_pr, nc * sizeof(uint64_t));
            if (!t) exit(EXIT_FAILURE);
            A083174_pr = t;
            A083174_cap = nc;
        }
        uint64_t c = A083174_len ? A083174_pr[A083174_len - 1] + 1 : 2;
        while (A083174_len < need) {
            bool ok = true;
            for (size_t i = 0; i < A083174_len; ++i) {
                uint64_t p = A083174_pr[i];
                if (p * p > c) break;
                if (c % p == 0) { ok = false; break; }
            }
            if (ok) A083174_pr[A083174_len++] = c;
            ++c;
        }
    }
    return A083174_pr[need - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t p = (seq_t)A083174_prime(n);
    seq_t k = (seq_t)n;
    if ((n & 1) == 0) k = k + k / 2;
    return p * k;
}
