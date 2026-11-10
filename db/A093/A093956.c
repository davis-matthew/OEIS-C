/*
 * A093956  a(n) = A091787(n) - 2.
 * https://oeis.org/A093956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091787(n) - 2, where A091787(1) = 2 and A091787(n+1) =
 *            max(k,2), k = max number of repeated end blocks of
 *            A091787(1..n); computed by direct search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 8110 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A091787_seq = NULL;
static idx_t A091787_len = 0;
static idx_t A091787_cap = 0;

static void A091787_extend(idx_t n)
{
    if (n > A091787_cap) {
        idx_t nc = A091787_cap ? A091787_cap : 64;
        while (nc < n) nc *= 2;
        A091787_seq = mem_resize(seq_t, A091787_seq, nc);
        A091787_cap = nc;
    }
    if (A091787_len == 0) {
        A091787_seq[0] = 2;
        A091787_len = 1;
    }
    while (A091787_len < n) {
        idx_t len = A091787_len;
        idx_t max_k = 1;
        for (idx_t L = 1; L <= len / 2 && len / L > max_k; ++L) {
            idx_t pos = len - L;
            idx_t k = 1;
            while (pos >= L) {
                int eq = 1;
                for (idx_t j = 0; j < L; ++j) {
                    if (A091787_seq[pos - L + j] != A091787_seq[pos + j]) { eq = 0; break; }
                }
                if (!eq) break;
                ++k;
                pos -= L;
            }
            if (k > max_k) max_k = k;
        }
        A091787_seq[len] = (seq_t)(max_k > 2 ? max_k : 2);
        A091787_len = len + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A091787_extend(n);
    return A091787_seq[n - 1] - 2;
}
