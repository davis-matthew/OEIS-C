/*
 * A044233  Numbers n such that string 5,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose base‑8 representation
   contains the substring "56" but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x in base 8 contains the consecutive digits 5,6. */
static bool has56_oct(seq_t x)
{
    /* Extract digits of x in base 8 into a buffer. */
    unsigned char buf[128];
    int len = 0;
    while (x > (seq_t)0) {
        buf[len++] = (unsigned char)(x % (seq_t)8);
        x /= (seq_t)8;
    }
    if (len < 2) return false;
    /* Scan from most significant to least significant digit. */
    for (int i = len - 1; i > 0; --i) {
        if (buf[i] == (unsigned char)5 && buf[i-1] == (unsigned char)6)
            return true;
    }
    return false;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* behaviour below offset is irrelevant */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;          /* last integer examined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cur = last_checked + (seq_t)1;
        bool cur_has  = has56_oct(cur);
        bool prev_has = has56_oct(cur - (seq_t)1);

        if (cur_has && !prev_has) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();             /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }

        last_checked = cur;
    }

    return cache[n - 1];
}
