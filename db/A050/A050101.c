/*
 * A050101  a(n) is the position of n in A050100.
 * https://oeis.org/A050101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050100, where A050100 has b(1)=1 and
 *            b(k) = floor(b(k-1)/3) if that is positive and not yet used,
 *            else 10*b(k-1); computed by generating A050100 until n appears
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050100_c = NULL;
static size_t A050100_len = 0;
static size_t A050100_cap = 0;

static void A050100_push(seq_t v)
{
    if (A050100_len == A050100_cap) {
        size_t nc = A050100_cap ? A050100_cap * 2 : 64;
        seq_t *p = mem_new(seq_t, nc);
        for (size_t i = 0; i < A050100_len; ++i)
            p[i] = A050100_c[i];
        if (A050100_c) mem_free(A050100_c);
        A050100_c = p;
        A050100_cap = nc;
    }
    A050100_c[A050100_len++] = v;
}

static bool A050100_step(void)
{
    if (A050100_len == 0) {
        A050100_push((seq_t)1);
        return true;
    }
    seq_t prev = A050100_c[A050100_len - 1];
    seq_t cand = prev / (seq_t)3;
    bool used = !(cand > (seq_t)0);
    if (!used) {
        for (size_t i = 0; i < A050100_len; ++i) {
            if (A050100_c[i] == cand) { used = true; break; }
        }
    }
    seq_t nv = used ? prev * (seq_t)10 : cand;
    if (!(nv > (seq_t)0)) return false; /* overflow wrapped to zero */
    A050100_push(nv);
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t target = (seq_t)n;
    size_t scanned = 0;
    for (;;) {
        while (scanned < A050100_len) {
            if (A050100_c[scanned] == target)
                return (seq_t)(scanned + 1);
            ++scanned;
        }
        if (!A050100_step()) return (seq_t)0;
    }
}
