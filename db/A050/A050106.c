/*
 * A050106  Numbers k such that A050104(k) < A050104(k+1).
 * https://oeis.org/A050106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A050104(k) < A050104(k+1); computed by
 *            direct generation of A050104 (a(1)=1; floor(a(n-1)/4) if not 0
 *            or earlier term, else 3*a(n-1)) and scanning for ascents
 * Range:     u8 n <= 30, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050104_buf = NULL;
static idx_t A050104_cap = 0;
static idx_t A050104_len = 0;

static void A050104_extend(void)
{
    if (A050104_len >= A050104_cap) {
        idx_t ncap = A050104_cap ? A050104_cap * 2 : 256;
        if (A050104_buf == NULL) {
            A050104_buf = mem_new(seq_t, ncap);
        } else {
            A050104_buf = mem_resize(seq_t, A050104_buf, ncap);
        }
        A050104_cap = ncap;
    }
    if (A050104_len == 0) {
        A050104_buf[0] = (seq_t)1;
        A050104_len = 1;
        return;
    }
    idx_t i = A050104_len;
    seq_t prev = A050104_buf[i - 1];
    seq_t cand = prev / (seq_t)4;
    bool used = false;
    if (cand == (seq_t)0) {
        used = true;
    } else {
        for (idx_t j = 0; j + 1 < i; ++j) {
            if (A050104_buf[j] == cand) { used = true; break; }
        }
    }
    A050104_buf[i] = used ? (seq_t)(prev * (seq_t)3) : cand;
    A050104_len = i + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    idx_t k = 1;
    for (;;) {
        while (A050104_len < k + 1) A050104_extend();
        if (A050104_buf[k - 1] < A050104_buf[k]) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
