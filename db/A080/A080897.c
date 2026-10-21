/*
 * A080897  Nonnegative integers written in "base American money" - the
 *          places, from the right, are pennies, nickels, dimes, quarters,
 *          50-cent pieces, dollars, 2 dollars, 5 dollars, 10 dollars, 20
 *          dollars, 50 dollars, 100 dollars, ...
 * https://oeis.org/A080897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the minimal-coin representation of n in
   American money denominations.  The value is obtained by a greedy
   change algorithm over the sequence
   1, 5, 10, 25, 50, 100, 200, 500, 1000, 2000, 5000, 10000, ...
   which continues with a multiplier pattern of 5/2, 2, 2 repeating
   after the first 200.  Each digit of the result is the number of
   coins of that denomination in the optimal solution. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base American coin denominations up to 1 dollar. */
    static const seq_t base[] = {1, 5, 10, 25, 50, 100};
    const size_t base_len = sizeof(base) / sizeof(base[0]);

    /* Storage for all denominations needed for this n.  For n <= 10000
       we need at most 12 denominations. */
    seq_t coins[20];
    size_t count = 0;

    /* Copy the base denominations into our array. */
    for (size_t i = 0; i < base_len; ++i) {
        if (base[i] > (seq_t)n) break;
        coins[count++] = base[i];
    }

    /* Generate further denominations if required.  After 100, the
       first extra coin is 200 (multiply by 2).  From then on the
       multiplier pattern is 5/2, 2, 2 repeating. */
    if (n > 100) {
        seq_t prev = 100;
        /* First extra coin: 200. */
        seq_t next = prev * 2;
        if (next <= (seq_t)n) {
            coins[count++] = next;
            prev = next;
            /* Pattern index: 0 -> 5/2, 1 -> 2, 2 -> 2, then repeat. */
            int pat = 0;
            while (1) {
                if (pat == 0) {
                    /* Multiply by 5/2.  prev is always even, so the
                       result is an integer. */
                    next = prev * 5 / 2;
                } else {
                    next = prev * 2;
                }
                if (next > (seq_t)n || next <= prev) break;
                coins[count++] = next;
                prev = next;
                pat = (pat + 1) % 3;
            }
        }
    }

    /* Greedy change: from largest to smallest denomination. */
    idx_t rem = n;          /* remainder to be represented. */
    seq_t result = 0;       /* the output number. */

    for (size_t i = count; i > 0; --i) {
        seq_t coin = coins[i - 1];
        /* Count of this coin in the optimal representation. */
        seq_t cnt = (seq_t)rem / coin;
        rem -= (idx_t)(cnt * coin);
        result = result * 10 + cnt;
    }

    return result;
}
