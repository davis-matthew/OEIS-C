/*
 * A037877  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*9^i} is base-9
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d_i – e_i|, where d_i are the base‑9 digits of
 *            n (least significant first) and e_i are the digits sorted in
 *            non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is O(number_of_digits(n)) and works for any n ≥ 1. */
#include <stdint.h>

/* Return the A037877 sequence value for index n.  */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (not part of the sequence,
       but harmless to support). */
    if (n == 0)
        return (seq_t)0;

    /* Base‑9 digits of n:  n%9 gives a digit 0..8.  */
    /* The maximum number of base‑9 digits for a 64‑bit integer is 21,
       so a fixed array of size 64 is safely large. */
    uint8_t d[64];
    uint8_t e[64];
    unsigned int m = 0;          /* digit count */

    idx_t tmp = n;                /* work on a copy of n */
    while (tmp > 0) {
        d[m] = (uint8_t)(tmp % 9);
        tmp  = (idx_t)(tmp / 9);
        ++m;
    }

    /* Copy to e for sorting. */
    for (unsigned int i = 0; i < m; ++i)
        e[i] = d[i];

    /* Simple insertion sort in descending order.  Complexity negligible
       for m <= 64. */
    for (unsigned int i = 1; i < m; ++i) {
        uint8_t key = e[i];
        int j = (int)i - 1;
        while (j >= 0 && e[j] < key) {
            e[j + 1] = e[j];
            --j;
        }
        e[j + 1] = key;
    }

    /* Accumulate absolute differences. */
    seq_t sum = 0;
    for (unsigned int i = 0; i < m; ++i) {
        seq_t diff = (seq_t)d[i] > (seq_t)e[i] ? (seq_t)d[i] - (seq_t)e[i] 
                                                : (seq_t)e[i] - (seq_t)d[i];
        sum += diff;
    }

    /* The sum is guaranteed to be even, so integer division is exact. */
    return sum / (seq_t)2;
}
