/*
 * A005380  Expansion of 1 / Product_{k>=1} (1-x^k)^(k+1).
 * https://oeis.org/A005380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} (sigma_1(k) + sigma_2(k)) * a(n-k),
 *            a(0) = 1 (Euler transform of b(n) = n+1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 39, u64 n <= 105, u128 n <= 288,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width, sigma_sum,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/sigma_sum.h"

static uint64_t *sig_cache = NULL;
static idx_t sig_len = 0;

static unsigned __int128 *fmemo = NULL;
static idx_t fmemo_len = 0;

static seq_t *bmemo = NULL;
static idx_t bmemo_len = 0;

static void ensure_sigma(idx_t n) {
    idx_t k;
    if (sig_len > n) return;
    sig_cache = (uint64_t *)realloc(sig_cache, (size_t)(n + 1) * sizeof(uint64_t));
    for (k = sig_len; k <= n; k++) sig_cache[k] = (k == 0) ? 0 : sigma_sum(k);
    sig_len = n + 1;
}

seq_t a(idx_t n) {
    idx_t i, k;
    ensure_sigma(n);
    if (seq_is_fixed_width()) {
        /* exact for every value that fits in 64 bits: n*a(n) stays below 2^128 */
        if (fmemo_len <= n) {
            fmemo = (unsigned __int128 *)realloc(fmemo, (size_t)(n + 1) * sizeof(unsigned __int128));
            if (fmemo_len == 0) {
                fmemo[0] = 1;
                fmemo_len = 1;
            }
            for (i = fmemo_len; i <= n; i++) {
                unsigned __int128 s = 0;
                for (k = 1; k <= i; k++)
                    s += (unsigned __int128)sig_cache[k] * fmemo[i - k];
                fmemo[i] = s / (unsigned __int128)i;
            }
            fmemo_len = n + 1;
        }
        return u128_to_seq(fmemo[n]);
    } else {
        if (bmemo_len <= n) {
            idx_t old = bmemo_len;
            if (bmemo == NULL) {
                bmemo = alloc_zero(n + 1);
                bmemo[0] = 1;
                old = 1;
            } else {
                bmemo = grow_zero(bmemo, bmemo_len, n + 1);
            }
            bmemo_len = n + 1;
            for (i = old; i <= n; i++) {
                seq_t s = 0;
                for (k = 1; k <= i; k++)
                    s += bmemo[i - k] * (seq_t)sig_cache[k];
                bmemo[i] = s / (seq_t)i;
            }
        }
        return bmemo[n];
    }
}
