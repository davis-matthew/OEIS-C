/*
 * A050607  Numbers k such that base 5 expansion matches
 *          (0|1|2)*((0|1)(3|4))?(0|1|2)*.
 * https://oeis.org/A050607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050607: numbers s such that C(2s-1,s) is not divisible by 25.
   This is equivalent to the number of carries when adding s and (s-1) in
   base‑5 being at most 1 (Kummer's theorem).  The function generates the
   sequence by scanning integers, testing the carry property, and
   memoising the results. */

/* Include needed standard header */
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: true if s satisfies the carry condition */
static inline int qualifies_idx(idx_t s)
{
    if (s == 0) return 1;            /* 0 is a term of the sequence */

    idx_t a = s;
    idx_t b = s - 1;
    int  carry = 0;
    int  carr_cnt = 0;

    while (a > 0 || b > 0 || carry)
    {
        int da = a % 5;
        int db = b % 5;

        int sum = da + db + carry;
        if (sum >= 5)
        {
            carry = 1;
            carr_cnt++;
            if (carr_cnt > 1) return 0;   /* more than one carry */
        }
        else
            carry = 0;

        a /= 5;
        b /= 5;
    }
    return 1;   /* carries <= 1 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* static candidate to test next */
    static idx_t next_candidate = 0;

    /* Expand cache until it contains at least n terms */
    while (cache_size < n)
    {
        if (qualifies_idx(next_candidate))
        {
            /* ensure capacity */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;          /* allocation failure, return 0 */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
