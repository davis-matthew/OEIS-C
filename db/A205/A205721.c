/*
 * A205721  The number j such that 10 divides prime(k)-prime(j), where
 *          k(n)=A205720(n).
 * https://oeis.org/A205721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j, where the pairs (k, j) with j<k and prime(k) ≡
 *            prime(j) (mod 10) are listed by increasing k, then increasing j
 *            (incremental prime generation)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A205721_pr = NULL;
static size_t A205721_np = 0, A205721_pcap = 0;
static uint64_t *A205721_sq = NULL;
static size_t A205721_ns = 0, A205721_scap = 0;

static void A205721_push_seq(uint64_t v)
{
    if (A205721_ns == A205721_scap) {
        A205721_scap = A205721_scap ? A205721_scap * 2 : 256;
        A205721_sq = (uint64_t *)realloc(A205721_sq, A205721_scap * sizeof(uint64_t));
    }
    A205721_sq[A205721_ns++] = v;
}

static void A205721_next(void)
{
    uint64_t c = A205721_np ? A205721_pr[A205721_np - 1] + 1 : 2;
    for (;; ++c) {
        bool ok = true;
        for (size_t i = 0; i < A205721_np; ++i) {
            uint64_t p = A205721_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok && c >= 2) break;
    }
    if (A205721_np == A205721_pcap) {
        A205721_pcap = A205721_pcap ? A205721_pcap * 2 : 64;
        A205721_pr = (uint64_t *)realloc(A205721_pr, A205721_pcap * sizeof(uint64_t));
    }
    A205721_pr[A205721_np++] = c;
    for (size_t j = 0; j + 1 < A205721_np; ++j)
        if (A205721_pr[j] % 10 == c % 10)
            A205721_push_seq((uint64_t)(j + 1));
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A205721_ns < (size_t)n)
        A205721_next();
    return (seq_t)A205721_sq[n - 1];
}
