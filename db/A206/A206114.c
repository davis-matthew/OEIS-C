/*
 * A206114  Number of (n+1) X 5 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 309 * a(n-3) for n > 7.
   Initial values:
   a(1)=31756, a(2)=111552, a(3)=520800, a(4)=5048352, a(5)=21702240,
   a(6)=157206240, a(7)=1542266208.  This implementation uses the
   given empirical recurrence and is memoised only for the first
   seven terms.  The function works for any seq_t as long as the
   underlying operations respect the usual C truncation rules. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        0ULL,
        (seq_t)31756ULL,
        (seq_t)111552ULL,
        (seq_t)520800ULL,
        (seq_t)5048352ULL,
        (seq_t)21702240ULL,
        (seq_t)157206240ULL,
        (seq_t)1542266208ULL
    };

    if (n <= 7)
        return init[(size_t)n];

    idx_t k = n;
    while (k > 7)
        k -= 3;
    seq_t res = init[(size_t)k];
    while (k < n) {
        res *= (seq_t)309;
        k += 3;
    }
    return res;
}
