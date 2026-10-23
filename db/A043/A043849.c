/*
 * A043849  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 6 mod 7.
 * https://oeis.org/A043849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑4 representation has the
 *            number of runs congruent to 6 modulo 7. The function generates
 *            terms on demand by brute‑force search
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return true if the number of runs in the base‑4 representation of x
   is congruent to 6 modulo 7. */
static bool runs_mod7(seq_t x)
{
    seq_t prev = (seq_t)-1;  /* outside the digit set {0,1,2,3} */
    int runs = 0;
    while (x > 0) {
        seq_t d = x & (seq_t)3;
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x >>= 2;           /* divide by 4 */
    }
    return (runs % 7) == 6;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static seq_t next_num = 1;

    if (n <= sz) return cache[n-1];

    if (sz == cap) {
        size_t new_cap = cap ? cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    while (sz < n) {
        while (1) {
            if (runs_mod7(next_num)) {
                cache[sz++] = next_num;
                break;
            }
            ++next_num;
        }
        ++next_num;  /* start next search from next number */
    }
    return cache[n-1];
}
