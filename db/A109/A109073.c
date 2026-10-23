/*
 * A109073  Number of decimal digits (counted with multiplicity) in n^2 but
 *          not in n.
 * https://oeis.org/A109073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of decimal digits (with multiplicity) in n^2 that
 *            do NOT appear in n
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute n^2 as seq_t to allow arbitrary precision when needed */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;

    /* record which decimal digits occur in n */
    unsigned char present[10] = {0};
    idx_t tmpn = n;
    if (tmpn == 0) {
        present[0] = 1;
    } else {
        while (tmpn != 0) {
            unsigned d = (unsigned)(tmpn % 10);
            present[d] = 1;
            tmpn /= 10;
        }
    }

    /* count digits in n^2 that are not present in n */
    int count = 0;
    seq_t tmp = n2;
    while (tmp != (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        if (!present[d]) {
            ++count;
        }
        tmp /= (seq_t)10;
    }

    return (seq_t)count;
}
