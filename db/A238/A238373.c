/*
 * A238373  The number of "topped inward" shuffles to reorder a stack of n
 *          cards to its original order.
 * https://oeis.org/A238373
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the order of the topped‑inward shuffle permutation
 * for n cards.  That is, the smallest positive integer m such that
 * performing the shuffle m times returns the deck to its original order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 3. */
    if (n < 3) {
        return 0;
    }

    /* Arrays used to build the result of one shuffle and to hold the
     * resulting permutation map. */
    idx_t *order = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *perm  = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    unsigned char *vis = (unsigned char *)malloc((size_t)(n + 1) * sizeof(unsigned char));
    if (!order || !perm || !vis) {
        free(order); free(perm); free(vis);
        return 0;
    }

    /* Build the order after one topped‑inward shuffle.
     * order[0] holds the top card (card 1) which is considered the marked
     * card.  Subsequent cards are inserted alternately before or after
     * the marked one, starting with card 2 which is inserted after it. */
    idx_t cur_len = 1;
    idx_t marked_pos = 0;          /* index of the marked card in 'order' */
    order[0] = 1;                  /* top card is 1 */

    for (idx_t card = 2; card <= n; ++card) {
        /* Even cards go after the marked card; odd cards go before it. */
        idx_t pos = (card % 2 == 0) ? (marked_pos + 1) : marked_pos;

        /* Shift elements from pos to cur_len-1 one position to the right. */
        for (idx_t i = cur_len; i > pos; --i) {
            order[i] = order[i - 1];
        }
        order[pos] = card;
        ++cur_len;

        /* Inserting an odd card before the marked one pushes the marked
         * card one position to the right. */
        if (card % 2 == 1) {
            ++marked_pos;
        }
    }

    /* Convert the new order into a permutation map perm[i] = new position
     * (1‑based) of card i+1.  This facilitates cycle detection. */
    for (idx_t pos = 1; pos <= n; ++pos) {
        idx_t card = order[pos - 1];
        perm[card - 1] = pos;
    }

    /* Determine the order of the permutation by finding the lcm of the
     * lengths of its disjoint cycles. */
    seq_t lcm_val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        vis[i] = 0;
    }

    for (idx_t i = 1; i <= n; ++i) {
        if (vis[i]) continue;
        idx_t cycle_len = 0;
        idx_t j = i;
        do {
            vis[j] = 1;
            j = perm[j - 1];
            ++cycle_len;
        } while (j != i);

        if (cycle_len > 0) {
            seq_t g = gcd(lcm_val, (seq_t)cycle_len);
            lcm_val = (lcm_val / g) * (seq_t)cycle_len;
        }
    }

    free(order);
    free(perm);
    free(vis);

    return lcm_val;
}
