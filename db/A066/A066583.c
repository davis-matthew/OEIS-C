/*
 * A066583  Product of nonzero digits of A066551(n).
 * https://oeis.org/A066583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = product of nonzero digits of A066551(n), where A066551(n)
 *            is the n-digit block of the concatenation 1,4,9,16,25,...
 *            starting at digit n(n-1)/2+1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A066583_dig = NULL;
static size_t A066583_len = 0, A066583_cap = 0;
static uint64_t A066583_next = 1;

static void A066583_ensure(size_t needed)
{
    if (A066583_len >= needed) return;
    if (A066583_cap == 0) A066583_cap = 64;
    while (A066583_cap < needed + 32) A066583_cap <<= 1;
    uint8_t *t = (uint8_t *)realloc(A066583_dig, A066583_cap);
    if (!t) abort();
    A066583_dig = t;
    while (A066583_len < needed) {
        uint64_t sq = A066583_next * A066583_next;
        ++A066583_next;
        uint8_t buf[24];
        int pos = 0;
        do { buf[pos++] = (uint8_t)(sq % 10); sq /= 10; } while (sq);
        for (int k = pos - 1; k >= 0; --k)
            A066583_dig[A066583_len++] = buf[k];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t start = (size_t)(n * (n - 1) / 2);
    size_t cnt = (size_t)n;
    A066583_ensure(start + cnt);
    seq_t r = 1;
    for (size_t k = 0; k < cnt; ++k) {
        uint8_t d = A066583_dig[start + k];
        if (d > 1) r = r * (seq_t)d;
    }
    return r;
}
