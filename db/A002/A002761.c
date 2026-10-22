/*
 * A002761  Number of ways of getting a royal flush, other straight flush, 4
 *          of a kind, full house, other flush, other straight, 3 of a kind, 2
 *          pair, a pair or nothing in 5-card poker.
 * https://oeis.org/A002761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   exhaustive search -- enumerate all binomial(52,5) five-card
 *            hands, classify each one (royal flush, other straight flush,
 *            four of a kind, full house, other flush, other straight, three
 *            of a kind, two pair, one pair, nothing) and count each class;
 *            a(n) = count of class n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   a002761_classify
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002761_classify.h"

static uint32_t a002761_cnt[10];
static bool a002761_done = false;

static void a002761_build(void) {
    int c[5];
    for (c[0] = 0; c[0] < 52; c[0]++)
    for (c[1] = c[0] + 1; c[1] < 52; c[1]++)
    for (c[2] = c[1] + 1; c[2] < 52; c[2]++)
    for (c[3] = c[2] + 1; c[3] < 52; c[3]++)
    for (c[4] = c[3] + 1; c[4] < 52; c[4]++)
        a002761_cnt[a002761_classify(c)]++;
    a002761_done = true;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 10) return (seq_t)0;
    if (!a002761_done) a002761_build();
    return (seq_t)a002761_cnt[n - 1];
}
