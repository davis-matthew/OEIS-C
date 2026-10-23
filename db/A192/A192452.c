/*
 * A192452  Numbers n such that -1 is not a 4th power mod n.
 * https://oeis.org/A192452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A192452: Numbers n such that –1 is not a 4th power modulo n.
   The function returns the nth such number (a(1)=3).  The test is:
   - Let n factor as ∏ p_i^e_i.
   - For every odd prime p_i:  –1 is a 4th power modulo p_i^e_i iff p_i ≡ 1 (mod 8).
   - For the factor 2:  –1 is a 4th power modulo 2^e iff e == 1.
   Thus n is excluded (–1 is a 4th power mod n) precisely when
   all prime power factors satisfy the above.  All other n are included. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return true iff –1 is a 4th power modulo m (i.e. m is excluded). */
static bool is_excluded(uint64_t m)
{
    /* Factor out powers of 2. */
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e2;
        if (e2 > 1)          /* 2^k with k>=2 => inclusion */
            return false;
    }

    /* If m==1 after removing 2s, handle n=2 or n=1. */
    if (m == 1) {
        return true;          /* n==2 or n==1 are excluded */
    }

    /* Now test odd prime factors. */
    uint64_t d = 3;
    while (d <= m / d) {
        if (m % d == 0) {
            /* Extract full exponent. */
            while (m % d == 0) {
                m /= d;
            }
            if (d % 8ULL != 1ULL)   /* odd prime not ≡1 (mod 8) => inclusion */
                return false;
        }
        d += 2;
    }
    /* Remaining prime factor, if any. */
    if (m > 1 && (m % 8ULL != 1ULL))
        return false;

    /* All prime powers satisfy the condition ⇒ excluded. */
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t cand = 3;          /* next candidate to test */

    /* Grow and fill cache until we have n terms. */
    while ((idx_t)len < n) {
        uint64_t cur = cand;
        cand++;

        if (!is_excluded(cur)) {       /* include if –1 is NOT a 4th power */
            if (len + 1 > cap) {
                size_t new_cap = (cap == 0) ? 64 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)cur;
        }
    }
    return cache[n - 1];
}
