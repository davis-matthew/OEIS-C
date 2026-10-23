/*
 * A180963  Numbers divisible by 3 with an odd number of 1s in their base 2
 *          representation.
 * https://oeis.org/A180963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 3*A036556(n)
 * Range:     u8 n <= 16, u16 n <= 8736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence a(n) is the list of integers divisible by 3 whose binary
 * representation contains an odd number of 1‑bits.  We generate them
 * by scanning the multiples of 3 and keeping those with odd popcount,
 * returning the n‑th such value.
 */

#include <stdint.h> /* for uint64_t (used in the builtin popcount) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* n=0 is outside the defined range; return 0 */

    unsigned long long x = 3;    /* first positive multiple of 3 */
    idx_t count = 0;

    while (1) {
        /* popcount of x; odd when least significant bit is 1 */
        if ((__builtin_popcountll(x) & 1U) != 0) {
            ++count;
            if (count == n)
                return (seq_t)x; /* value fits in seq_t for the required range */
        }
        x += 3;                   /* next multiple of 3 */
    }
}
