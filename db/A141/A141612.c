/*
 * A141612  Write down 0,1,2,3,...n each in binary. Total up the number of 1's
 *          in each bit-position (total number of 1's in 1's position, total
 *          number of 1's in 2's position, total number of 1's in 4's
 *          position, etc.). a(n) = the number of such totals that each do not
 *          equal any other such total.
 * https://oeis.org/A141612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2825, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of unique totals of 1's per bit position when writing 0..n in binary */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly */
    if (n == 0)
        return 0;

    /* Determine number of bits needed to represent n */
    idx_t tmp = n;
    idx_t L = 0;
    while (tmp) {
        L++;
        tmp >>= 1;
    }

    /* Array to hold counts of 1's in each bit position */
    seq_t counts[65];               /* 65 > maximum bits for 64‑bit value */

    idx_t np1 = n + 1;              /* n + 1 used frequently */

    /* Compute counts for each bit position */
    for (idx_t k = 0; k < L; k++) {
        idx_t bits   = k + 1;
        idx_t cycle  = ((idx_t)1) << bits;
        idx_t full   = np1 >> bits;
        idx_t ones   = full << k;
        idx_t rem    = np1 & (cycle - 1);
        idx_t thresh = ((idx_t)1) << k;

        if (rem > thresh)
            ones += rem - thresh;

        counts[k] = (seq_t)ones;
    }

    /* Count how many of those totals are unique */
    seq_t unique = 0;
    for (idx_t i = 0; i < L; i++) {
        bool uniq = true;
        for (idx_t j = 0; j < L; j++) {
            if (i != j && counts[i] == counts[j]) {
                uniq = false;
                break;
            }
        }
        if (uniq)
            unique++;
    }

    return unique;
}
