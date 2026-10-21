/*
 * A235460  a(1)=2. For n>1, assume a(n-1) has decimal expansion abcdefghij
 *          (with at most ten digits, where some of j,i,h, ... may be
 *          missing). Then a(n) = sum 11...1 (with a 1's) + 22...2 (with b
 *          2's) + 33...3 (with c 3's) + ...
 * https://oeis.org/A235460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1 let a(n‑1) have decimal digits
   d1d2…dt (most significant first). Then
   a(n) = Σ_{p=1}^{t} (p repeated dp times). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"

/* Extract decimal digits of x into buf (MSD first).
   len is set to the number of digits. */
static void
get_digits(seq_t x, unsigned char *buf, size_t *len)
{
    if (x == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    size_t pos = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        buf[pos++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    /* reverse to most‑significant first */
    for (size_t i = 0; i < pos / 2; ++i) {
        unsigned char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    *len = pos;
}

/* Compute a(n), n ≥ 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = 2 */
    static size_t cap = 0;
    static idx_t max_cached = 0;
    static bool init = false;

    if (!init) {
        cap = 2;                    /* indices 0 and 1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;               /* unused */
        cache[1] = 2;               /* a(1) */
        max_cached = 1;
        init = true;
    }

    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    if (n <= max_cached) return cache[n];

    unsigned char digits[10];
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        size_t len;
        get_digits(prev, digits, &len);

        seq_t sum = 0;
        for (size_t pos = 1; pos <= len; ++pos) {
            unsigned char d = digits[pos - 1];
            if (d == 0) continue;

            size_t len_of_pos = (pos >= 10) ? 2 : 1;
            seq_t mult = pow10((idx_t)len_of_pos);
            seq_t rep = 0;
            for (unsigned int t = 0; t < d; ++t) {
                rep = rep * mult + (seq_t)pos;
            }
            sum += rep;
        }
        cache[i] = sum;
    }
    max_cached = n;
    return cache[n];
}
