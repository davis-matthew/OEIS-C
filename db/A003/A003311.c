/*
 * A003311  Write down the numbers from 3 to infinity. Take next number, M
 *          say, that has not been crossed off. Counting through the numbers
 *          that have not yet been crossed off after that M, cross off the
 *          first, (M+1)st, (2M+1)st, (3M+1)st, etc. Repeat. The numbers that
 *          are left form the sequence.
 * https://oeis.org/A003311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve, computed backwards. a(1) = 3. Stage k (k = 0, 1, ...)
 *            uses M = a(k+1) and removes, after 0-based list position k, the
 *            offsets r = 0, M, 2M, ... So position i after stage k came from
 *            position k+2+q+floor(q/(M-1)), where q = i-k-1. Start from i =
 *            n-1, undo stages k = n-2 down to 0, and retur...
 * Range:     u8 n <= 44, u16 n <= 5514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A3311_cache = NULL;
static idx_t A3311_len = 0, A3311_cap = 0;

static uint64_t A3311_compute(idx_t n)
{
    uint64_t i = (uint64_t)(n - 1);
    idx_t k;
    for (k = n - 1; k-- > 0;) {
        uint64_t M = A3311_cache[k];
        uint64_t q = i - (uint64_t)k - 1;
        i = (uint64_t)k + 2 + q + q / (M - 1);
    }
    return i + 3;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A3311_cap) {
        idx_t nc = A3311_cap ? A3311_cap : 64;
        while (nc < n) nc *= 2;
        A3311_cache = (uint64_t *)realloc(A3311_cache, sizeof(uint64_t) * nc);
        A3311_cap = nc;
    }
    while (A3311_len < n) {
        A3311_cache[A3311_len] = A3311_compute(A3311_len + 1);
        A3311_len++;
    }
    return (seq_t)A3311_cache[n - 1];
}
