/*
 * A105498  Trajectory of 1 under the morphism 1->{1,2}, 2->{1,4}, 3->{3,4},
 *          4->{3,4}.
 * https://oeis.org/A105498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol in the infinite word obtained by
 *            iterating the morphism 1 → 1,2 2 → 1,4 3 → 3,4 4 → 3,4 starting
 *            from the single symbol 1. The value a(n) belongs to {1,2,3,4}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* mapping table: next[prev][bit] gives the symbol that follows 'prev'
   when the next binary digit of the index is 'bit'.                           */
static const unsigned int next_sym[5][2] = {
    {0,0},          /* unused index 0 */
    {1,2},          /* 1 → 1 if bit=0, 2 if bit=1 */
    {1,4},          /* 2 → 1 if bit=0, 4 if bit=1 */
    {3,4},          /* 3 → 3 if bit=0, 4 if bit=1 */
    {3,4}           /* 4 → 3 if bit=0, 4 if bit=1 */
};

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    unsigned int cur = 1;      /* start from the root symbol 1 */

    /* find position of the most significant 1‑bit in n (n > 0) */
    unsigned int msb = 0;
    for (int i = 63; i >= 0; --i) {
        if (((uint64_t)n >> i) & 1ULL) {
            msb = i;
            break;
        }
    }

    /* traverse the bits from most to least significant, updating the
       current symbol according to the morphism rules.                    */
    for (int i = msb; i >= 0; --i) {
        unsigned int bit = (uint64_t)(( (uint64_t)n >> i) & 1ULL);
        cur = next_sym[cur][bit];
    }

    return (seq_t)cur;          /* cast to the required return type */
}
