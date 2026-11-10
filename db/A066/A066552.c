/*
 * A066552  Sum of digits of n-th term of A066551.
 * https://oeis.org/A066552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of A066551(n), where A066551(n) is
 *            the n-digit block of the concatenation 1,4,9,16,25,... (squares)
 *            starting at digit n(n-1)/2+1 (leading zeros add nothing to the
 *            sum)
 * Range:     u8 n <= 49, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A066552_dig = NULL;
static size_t A066552_len = 0;
static size_t A066552_cap = 0;
static uint64_t A066552_next = 1;

static void A066552_ensure(size_t needed)
{
    if (A066552_len >= needed)
        return;
    if (A066552_cap == 0)
        A066552_cap = 64;
    while (A066552_cap < needed + 32)
        A066552_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(A066552_dig, A066552_cap);
    if (!tmp)
        abort();
    A066552_dig = tmp;
    while (A066552_len < needed) {
        uint64_t sq = A066552_next * A066552_next;
        ++A066552_next;
        uint8_t buf[24];
        int pos = 0;
        do {
            buf[pos++] = (uint8_t)(sq % 10);
            sq /= 10;
        } while (sq);
        for (int k = pos - 1; k >= 0; --k)
            A066552_dig[A066552_len++] = buf[k];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t start = (uint64_t)n * ((uint64_t)n - 1) / 2;
    size_t needed = (size_t)start + (size_t)n;
    A066552_ensure(needed);
    uint64_t s = 0;
    for (uint64_t k = 0; k < (uint64_t)n; ++k)
        s += A066552_dig[(size_t)start + (size_t)k];
    return (seq_t)s;
}
