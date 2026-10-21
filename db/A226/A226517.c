/*
 * A226517  Number of (19,14)-reverse multiples with n digits.
 * https://oeis.org/A226517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = a(n-2) + a(n-8) for n > 11; initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=0, a(8)=0,
 *            a(9)=0, a(10)=1, a(11)=1
 * Range:     u8 n <= 45, u16 n <= 79, u32 n <= 149, u64 n <= 287,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_size)
        return;

    size_t old_size = a_cache_size;
    size_t new_size = (old_size == 0) ? 8 : old_size;
    while (new_size <= (size_t)n)
        new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();

    a_cache = tmp;
    a_cache_size = new_size;

    for (idx_t i = old_size; i < new_size; ++i) {
        if (i <= 11u) {
            switch ((int)i) {
                case 6:
                case 10:
                case 11:
                    a_cache[i] = 1;
                    break;
                default:
                    a_cache[i] = 0;
                    break;
            }
        } else {
            a_cache[i] = a_cache[i - 2] + a_cache[i - 8];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[n];
}
