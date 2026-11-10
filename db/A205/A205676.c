/*
 * A205676  Positions of multiples of 4 in A204890 (differences of primes).
 * https://oeis.org/A205676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th multiple of 4 in A204890, where
 *            A204890 is the triangle of differences prime(j)-prime(i),
 *            1<=i<j, read by rows (found by direct scan)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *A205676_pr = NULL;
static size_t A205676_psz = 0, A205676_pcap = 0;
static uint32_t A205676_cand = 3;

static void A205676_ensure(size_t k)
{
    if (A205676_pcap == 0) {
        A205676_pcap = 64;
        A205676_pr = (uint32_t *)malloc(A205676_pcap * sizeof(uint32_t));
        if (!A205676_pr) abort();
        A205676_pr[0] = 2;
        A205676_psz = 1;
    }
    while (A205676_psz < k) {
        uint32_t p = A205676_cand;
        bool ok = true;
        for (size_t i = 1; i < A205676_psz; ++i) {
            uint32_t q = A205676_pr[i];
            if ((uint64_t)q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        A205676_cand += 2;
        if (!ok) continue;
        if (A205676_psz == A205676_pcap) {
            A205676_pcap <<= 1;
            A205676_pr = (uint32_t *)realloc(A205676_pr, A205676_pcap * sizeof(uint32_t));
            if (!A205676_pr) abort();
        }
        A205676_pr[A205676_psz++] = p;
    }
}

static uint64_t *A205676_pos_arr = NULL;
static size_t A205676_cnt = 0, A205676_cap = 0;
static uint64_t A205676_j = 2, A205676_i = 1, A205676_pos = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A205676_cnt < (size_t)n) {
        A205676_pos++;
        A205676_ensure((size_t)A205676_j);
        uint32_t d = A205676_pr[A205676_j - 1] - A205676_pr[A205676_i - 1];
        if (d % 4 == 0) {
            if (A205676_cnt == A205676_cap) {
                A205676_cap = A205676_cap ? A205676_cap * 2 : 64;
                A205676_pos_arr = (uint64_t *)realloc(A205676_pos_arr, A205676_cap * sizeof(uint64_t));
                if (!A205676_pos_arr) abort();
            }
            A205676_pos_arr[A205676_cnt++] = A205676_pos;
        }
        A205676_i++;
        if (A205676_i == A205676_j) { A205676_j++; A205676_i = 1; }
    }
    return (seq_t)A205676_pos_arr[(size_t)n - 1];
}
