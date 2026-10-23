/*
 * A121033  Multiples of 13 containing a 13 in their decimal representation.
 * https://oeis.org/A121033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n-th multiple of 13 whose decimal representation contains the substring "13".
 * The sequence is defined by searching all multiples of 13 and selecting those with "13" in
 * their decimal notation.  This implementation performs an incremental search for the
 * required term.
 */

#include <stdbool.h>

static bool contains13(seq_t x)
{
    unsigned int prev = 10;          /* value outside digit range */
    while (x > 0) {
        unsigned int cur = (unsigned int)(x % 10);
        if (prev == 3 && cur == 1) return true;
        prev = cur;
        x = x / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    idx_t  count = 0;
    seq_t  k      = 1;               /* multiplier of 13 */
    seq_t  mult13 = 13;

    while (count < n) {
        seq_t val = mult13 * k;
        if (contains13(val)) {
            ++count;
            if (count == n) return val;
        }
        ++k;
    }
    return 0;                        /* unreachable for valid n */
}
