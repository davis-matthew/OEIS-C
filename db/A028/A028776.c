/*
 * A028776  Nonsquares mod 63.
 * https://oeis.org/A028776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th positive integer that is not a quadratic residue
 *            modulo 63
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    static bool squares[63];
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t cur = 1;

    if (!init) {
        for (idx_t i = 0; i < 63; i++) squares[i] = false;
        for (idx_t k = 0; k < 63; k++) squares[(k * k) % 63] = true;
        init = true;
    }

    while (len < n) {
        if (!squares[cur % 63]) {
            if (len >= cap) {
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cap = newcap;
            }
            cache[len] = (seq_t)cur;
            len++;
        }
        cur++;
    }
    return cache[n - 1];
}
