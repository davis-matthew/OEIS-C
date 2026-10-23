/*
 * A044181  Numbers n such that string 6,1 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: linear search in increasing integers.  For each candidate
 * we check its base‑7 representation for the substring “61”.
 * a(n) is the n‑th integer whose base‑7 expansion contains “61”
 * but whose predecessor does not.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Return true iff x in base 7 contains the digit pair 6‑1
 * (most significant 6 followed by 1).  Digits are stored with
 * least significant first; thus the pair appears as 1,6 in
 * the digits array. */
static bool has61(seq_t x)
{
    if (x == 0)
        return false;

    seq_t tmp = x;
    unsigned char digits[64];
    int len = 0;

    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 7);
        tmp /= 7;
    }

    for (int i = 0; i + 1 < len; ++i) {
        if (digits[i] == 1 && digits[i + 1] == 6)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is 1‑based; undefined for 0 */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t next_candidate = 1;

    while (cache_size < (size_t)n) {
        /* Find the next integer that satisfies the definition. */
        for (;;) {
            bool cur_has = has61((seq_t)next_candidate);
            bool prev_has = has61((seq_t)(next_candidate - 1));
            if (cur_has && !prev_has)
                break;
            ++next_candidate;
        }

        if (cache_size == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort();   /* allocation failure – unlikely */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
