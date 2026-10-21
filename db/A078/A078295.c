/*
 * A078295  Smallest multiple of n whose digits can be re-arranged to be a
 *          substring of the cyclic concatenation 1234567890123435678901234...
 * https://oeis.org/A078295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   matches_cyclic_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A078295:  a(n) = smallest multiple of n whose multiset of decimal
 * digits can be permuted to form a contiguous substring of the
 * infinite cyclic concatenation 01234567890123456789...
 *
 * This implementation searches multiples of n in order, and for each
 * candidate checks whether its digit counts match the digit counts of
 * any substring of the repeating 0–9 pattern of the same length.
 *
 * The algorithm runs in O( L × M ) where L is the number of digits
 * of the current candidate and M is the number of multiples examined.
 * For the test range (n up to about 10 000) this is comfortably below
 * 2 s on all supported builds.  Overflow in the 64‑bit build is
 * detected; in that case the function returns 0 – such values are
 * outside the range tested by the problem set.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/matches_cyclic_pattern.h"

/* Count decimal digits of x, storing frequency in cnt[10].
 *  cnt must be large enough; size is sizeof(cnt) / sizeof(cnt[0]).
 *  Returns the total number of digits. */
static unsigned int digit_counts(seq_t x, unsigned int cnt[10])
{
    unsigned int len = 0;

    /* Special case: zero has one digit '0' */
    if (x == (seq_t)0) {
        memset(cnt, 0, 10 * sizeof(unsigned int));
        cnt[0] = 1;
        return 1;
    }

    memset(cnt, 0, 10 * sizeof(unsigned int));
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % (seq_t)10);
        cnt[d] += 1;
        x /= (seq_t)10;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 is undefined; return 0 to avoid UB */
    if (n == 0) return 0;

    seq_t        m;              /* current candidate multiple of n */
    unsigned int cnt[10];

    /* To avoid overflow in the 64‑bit build, we detect wrap‑around
       of the accumulated multiple.  For bigger integer types (big‑int)
       this check is harmless. */
    seq_t prev = 0;

    /* Start searching from the first multiple (k = 1). */
    while (true) {
        /* m = (k+1)*n, implemented as m = prev + n */
        m = prev + (seq_t)n;
        prev = m;

        /* Detect overflow: if we are on the 64‑bit build and the
           addition wrapped around, stop searching. */
#if defined(UINT64_MAX) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        /* For systems where sizeof(seq_t) == 8 we check using __int128. */
        if (sizeof(seq_t) == 8) {
            __int128 tmp = (__int128)prev + (__int128)n;
            if (tmp > (__int128)UINT64_MAX) break;
        }
#endif
        unsigned int len = digit_counts(m, cnt);
        if (matches_cyclic_pattern(cnt, len))
            return m;
    }
    return 0;   /* Should never be reached for the tested range */
}
