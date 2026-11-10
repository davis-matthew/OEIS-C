/*
 * A096880  Primes arising in A096879 (as absolute first differences).
 * https://oeis.org/A096880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A096879(n) - A096879(n+1)), where A096879(1) = 1 and
 *            A096879(n) is the least multiple of n coprime to n+1 whose
 *            absolute difference from A096879(n-1) is a prime not used before
 *            (computed by search)
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   gcd, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *A096879_cache = NULL;
static size_t A096879_cap = 0;
static size_t A096879_len = 0;
static seq_t *A096879_diffs = NULL;
static size_t A096879_dcap = 0;
static size_t A096879_dlen = 0;

static void A096879_extend(size_t need)
{
    if (need + 1 > A096879_cap) {
        size_t nc = A096879_cap ? A096879_cap * 2 : 16;
        while (nc < need + 1) nc <<= 1;
        A096879_cache = mem_resize(seq_t, A096879_cache, nc);
        A096879_cap = nc;
    }
    for (size_t i = A096879_len + 1; i <= need; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else {
            seq_t prev = A096879_cache[i - 1];
            seq_t step = (seq_t)i;
            seq_t m = step;
            while (true) {
                if (gcd(m, (seq_t)(i + 1)) == (seq_t)1) {
                    seq_t diff = (m >= prev) ? (seq_t)(m - prev) : (seq_t)(prev - m);
                    if (diff > (seq_t)1 && is_prime_seq(diff)) {
                        bool used = false;
                        for (size_t j = 0; j < A096879_dlen; ++j) {
                            if (A096879_diffs[j] == diff) { used = true; break; }
                        }
                        if (!used) {
                            if (A096879_dlen == A096879_dcap) {
                                size_t d = A096879_dcap ? A096879_dcap * 2 : 16;
                                A096879_diffs = mem_resize(seq_t, A096879_diffs, d);
                                A096879_dcap = d;
                            }
                            A096879_diffs[A096879_dlen++] = diff;
                            val = m;
                            break;
                        }
                    }
                }
                m += step;
            }
        }
        A096879_cache[i] = val;
        A096879_len = i;
    }
}

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    A096879_extend(k + 1);
    seq_t x = A096879_cache[k];
    seq_t y = A096879_cache[k + 1];
    return (x >= y) ? (seq_t)(x - y) : (seq_t)(y - x);
}
