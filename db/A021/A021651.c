/*
 * A021651  Decimal expansion of 1/647.
 * https://oeis.org/A021651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/647, i.e. a(n) = ⌊ 10·r_{n} /
 *            647 ⌋ where r_0 = 1 and r_{k+1} = (10·r_k) mod 647
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cached_len = 0;
static uint64_t cached_rem = 1;

seq_t a(idx_t n)
{
    if (n < cached_len)
        return cache[n];

    size_t new_cap = cached_len ? cached_len : 64;
    while (new_cap <= n + 1)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;
    cache = tmp;

    size_t i = cached_len;
    uint64_t r = cached_rem;
    while (i <= n) {
        uint64_t digit = (r * 10ULL) / 647ULL;
        r = (r * 10ULL) % 647ULL;
        cache[i] = (seq_t)digit;
        ++i;
    }
    cached_len = i;
    cached_rem = r;
    return cache[n];
}
