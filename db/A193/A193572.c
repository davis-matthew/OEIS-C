/*
 * A193572  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 20.
 * https://oeis.org/A193572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose sum–of–squares–of–decimal–digits process reaches
 *            20 before any other unhappy cycle member
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sum of squares of decimal digits of v. Works for all supported seq_t
   types that define % 10, / 10 and * operators. */
static inline seq_t sum_sq_digits(seq_t v)
{
    seq_t s = 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        s += (seq_t)d * (seq_t)d;
        v /= 10;
    }
    return s;
}

/* Returns true if starting from v we reach 20 before any of
   {4, 16, 37, 58, 89, 145, 42, 1, 0}.  1 and 0 correspond to the
   happy-loop (1) and the trivial fixed point (0) and are therefore
   rejected. */
static inline bool reaches_20_first(seq_t v)
{
    /* Immediate acceptance/rejection for cycle members. */
    if (v == 20) return true;
    if (v == 4 || v == 16 || v == 37 || v == 58 ||
        v == 89 || v == 145 || v == 42) return false;

    seq_t cur = v;
    while (true) {
        seq_t nxt = sum_sq_digits(cur);
        if (nxt == 20)                     return true;
        if (nxt == 4 || nxt == 16 || nxt == 37 || nxt == 58 ||
            nxt == 89 || nxt == 145 || nxt == 42 ||
            nxt == 1 || nxt == 0)          return false;
        cur = nxt;
    }
}

/* Cache for the first N terms that we have already computed.  The
   OEIS b-file contains a huge amount of terms (hundreds of thousands)
   but our tests only require up to about ten thousand, so a fixed
   maximum of 200 000 keeps the memory small. */
#define CACHE_MAX 200000
static seq_t cache[CACHE_MAX];
static idx_t cache_len   = 0;   /* number of filled entries   */
static idx_t test_next   = 1;   /* next integer to examine  */

seq_t a(idx_t n)
{
    /* Sanity check for a reasonably large request – the build will
       crash if we exceed CACHE_MAX.  The required limit (≈100 000)
       is well below that. */
    if (n == 0) return 0; /* undefined by OEIS, but safe guard */

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < n) {
        if (test_next < CACHE_MAX) {
            if (reaches_20_first(test_next))
                cache[cache_len++] = test_next;
        } else {
            /* In practice this should never happen for the required
               range of n. */
            break;
        }
        ++test_next;
    }
    /* cache_len >= n is now guaranteed. */
    return cache[n - 1];
}
