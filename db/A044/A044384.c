/*
 * A044384  Numbers n such that string 5,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal representation of n contains
   the substring "52" but the decimal representation of n-1 does not. */

#include <stdlib.h>
#include <stdbool.h>

static bool has52(seq_t x)
{
    unsigned int prev = 10;   /* sentinel value > 9 */
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        x /= 10;
        if (d == 5 && prev == 2)
            return true;
        prev = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static idx_t cur    = 51;  /* last integer examined */

    while (size < n) {
        cur++;                       /* candidate integer */
        seq_t cand = (seq_t)cur;
        if (has52(cand) && !has52(cand - 1)) {
            if (size == cap) {
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[size++] = cand;
        }
    }
    return cache[n - 1];
}
