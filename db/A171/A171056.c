/*
 * A171056  In the sequence of prime numbers, replace all the '9' digits with
 *          '7' and vice versa.
 * https://oeis.org/A171056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = swap9_7(prime(n)) where prime(n) is the nth prime,
 *            swapping every '9' digit to '7' and every '7' digit to '9'
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t swap_digits(seq_t v)
{
    if (v == 0) return 0;

    /* buffer holding digits in reverse order (least‑significant first) */
    uint8_t buf[128];
    size_t len = 0;
    seq_t t = v;
    while (t > 0) {
        buf[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t d = buf[len - 1 - i];   /* most‑significant digit first */
        if (d == 7) d = 9;
        else if (d == 9) d = 7;
        res = res * 10 + d;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n==0 is undefined */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t cur   = 1;   /* last tested integer */

    if (len < n) {
        if (cap == 0) {
            cap = 32;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        while (len < n) {
            /* find the next prime */
            do { ++cur; } while (!is_prime_small(cur));

            seq_t val = swap_digits(cur);

            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = val;
        }
    }
    return cache[n - 1];
}
