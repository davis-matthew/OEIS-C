/*
 * A205718  Positions of multiples of 10 in A204890 (differences of primes).
 * https://oeis.org/A205718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k-1)*(k-2)/2 + i for the n-th pair i<k, in order of k
 *            then i, with prime(k) == prime(i) (mod 10); computed by
 *            incremental prime generation
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 985 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A205718_pos = NULL;
static size_t A205718_len = 0, A205718_cap = 0;
static uint64_t *A205718_pr = NULL;
static size_t A205718_np = 0, A205718_pcap = 0;

static void A205718_extend(void)
{
    uint64_t cand = A205718_np ? A205718_pr[A205718_np - 1] + 1 : 2;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < A205718_np; ++i) {
            uint64_t p = A205718_pr[i];
            if (p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) break;
        ++cand;
    }
    if (A205718_np == A205718_pcap) {
        A205718_pcap = A205718_pcap ? A205718_pcap * 2 : 64;
        A205718_pr = (uint64_t *)realloc(A205718_pr, A205718_pcap * sizeof(uint64_t));
        if (!A205718_pr) abort();
    }
    A205718_pr[A205718_np++] = cand;
    uint64_t j = (uint64_t)A205718_np;
    uint64_t r = cand % 10;
    uint64_t base = (j - 1) * (j - 2) / 2;
    for (size_t i = 0; i + 1 < A205718_np; ++i) {
        if (A205718_pr[i] % 10 == r) {
            if (A205718_len == A205718_cap) {
                A205718_cap = A205718_cap ? A205718_cap * 2 : 128;
                A205718_pos = (uint64_t *)realloc(A205718_pos, A205718_cap * sizeof(uint64_t));
                if (!A205718_pos) abort();
            }
            A205718_pos[A205718_len++] = base + (uint64_t)(i + 1);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A205718_len < (size_t)n)
        A205718_extend();
    return (seq_t)A205718_pos[(size_t)n - 1];
}
