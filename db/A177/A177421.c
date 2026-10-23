/*
 * A177421  a(n) = 6*a(n-1)-8*a(n-2)-9 for n > 10; a(0)=629, a(1)=6585,
 *          a(2)=26259, a(3)=221931, a(4)=1917027, a(5)=18285939,
 *          a(6)=125792217, a(7)=703932681, a(8)=7131271077, a(9)=26172260445,
 *          a(10)=103884128445.
 * https://oeis.org/A177421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) – 8*a(n-2) – 9 for n > 10; a(0)=629, a(1)=6585,
 *            a(2)=26259, a(3)=221931, a(4)=1917027, a(5)=18285939,
 *            a(6)=125792217, a(7)=703932681, a(8)=7131271077,
 *            a(9)=26172260445, a(10)=103884128445
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 23, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[11] = {
    (seq_t)629ULL,
    (seq_t)6585ULL,
    (seq_t)26259ULL,
    (seq_t)221931ULL,
    (seq_t)1917027ULL,
    (seq_t)18285939ULL,
    (seq_t)125792217ULL,
    (seq_t)703932681ULL,
    (seq_t)7131271077ULL,
    (seq_t)26172260445ULL,
    (seq_t)103884128445ULL
};

seq_t a(idx_t n)
{
    if (n <= 10) {
        return init[(size_t)n];
    }

    /* Compute a(n) for n > 10 using the linear recurrence.
       Only the two most recent terms are required. */
    seq_t prevprev = init[9];   /* a(9) */
    seq_t prev     = init[10];  /* a(10) */
    seq_t curr;

    for (idx_t i = 11; i <= n; ++i) {
        curr = 6 * prev - 8 * prevprev - 9;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
