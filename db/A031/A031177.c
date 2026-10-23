/*
 * A031177  Unhappy numbers: numbers having period-8 2-digitized sequences.
 * https://oeis.org/A031177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031177: Unhappy numbers; computed by sequential search for the n‑th unhappy number using sum‑of‑squares iteration. */
#include <stdbool.h>

static bool is_happy(seq_t x)
{
    static const unsigned char sq[10] = {0,1,4,9,16,25,36,49,64,81};

    while (x > 1 && x != 4) {          /* 4 starts the unhappy cycle */
        seq_t sum = (seq_t)0;
        seq_t y   = x;
        while (y > 0) {
            unsigned char d = (unsigned)(y % 10);   /* digit 0–9 */
            sum += sq[d];
            y   /= 10;
        }
        x = sum;
    }
    return x == 1;                     /* true if the number is happy */
}

seq_t a(idx_t n)
{
    idx_t  count = 0;                  /* how many unhappy numbers found */
    seq_t  cand  = (seq_t)2;           /* start from the smallest candidate >1 */

    for (;; ++cand) {
        if (!is_happy(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
