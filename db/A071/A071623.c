/*
 * A071623  Least k such that n = A071532(k).
 * https://oeis.org/A071623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that A071532(k) = n, where A071532(k) =
 *            2*#{j<=k : floor((3/2)^j) odd} - k; found by walking k upward
 *            (+-1 steps) and testing bit j of 3^j for parity of
 *            floor((3/2)^j)
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static uint64_t *A071623_w = NULL;   /* exact 3^k, little-endian 64-bit words */
static size_t A071623_nw = 0;
static size_t A071623_cap = 0;
static unsigned long long A071623_k = 0;
static long long A071623_s = 0;
static long long A071623_max = 0;
static seq_t *A071623_hit = NULL;
static size_t A071623_hcap = 0;

seq_t a(idx_t n)
{
    if (A071623_w == NULL) {
        A071623_cap = 64;
        A071623_w = (uint64_t *)malloc(A071623_cap * sizeof(uint64_t));
        A071623_w[0] = 1;
        A071623_nw = 1;
        A071623_hcap = 64;
        A071623_hit = mem_new(seq_t, A071623_hcap);
    }
    while ((unsigned long long)A071623_max < (unsigned long long)n) {
        /* multiply by 3 */
        unsigned __int128 carry = 0;
        for (size_t i = 0; i < A071623_nw; i++) {
            unsigned __int128 t = (unsigned __int128)A071623_w[i] * 3u + carry;
            A071623_w[i] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry) {
            if (A071623_nw >= A071623_cap) {
                A071623_cap *= 2;
                A071623_w = (uint64_t *)realloc(A071623_w, A071623_cap * sizeof(uint64_t));
            }
            A071623_w[A071623_nw++] = (uint64_t)carry;
        }
        A071623_k++;
        unsigned long long k = A071623_k;
        size_t wi = (size_t)(k >> 6);
        int bit = 0;
        if (wi < A071623_nw)
            bit = (int)((A071623_w[wi] >> (k & 63)) & 1u);
        A071623_s += bit ? 1 : -1;
        if (A071623_s > A071623_max) {
            A071623_max = A071623_s;
            if ((size_t)A071623_max >= A071623_hcap) {
                A071623_hcap *= 2;
                A071623_hit = mem_resize(seq_t, A071623_hit, A071623_hcap);
            }
            A071623_hit[(size_t)A071623_max] = (seq_t)k;
        }
    }
    return A071623_hit[(size_t)n];
}
