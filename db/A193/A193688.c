/*
 * A193688  Number of steps to reach 1 in Collatz (3x+1) problem starting with
 *          2^n - 1.
 * https://oeis.org/A193688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of steps to reach 1 in Collatz starting from 2^n − 1
 * Range:     u8 n <= 20, u16 n <= 4848, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_add1, big_init_from_2pow_minus1, big_is_one, big_mult3,
 *            big_shift_right, big_trailing_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_add1.h"
#include "../utils/big_init_from_2pow_minus1.h"
#include "../utils/big_is_one.h"
#include "../utils/big_mult3.h"
#include "../utils/big_shift_right.h"
#include "../utils/big_trailing_zeros.h"

#define MAXWORDS 4000  /* enough for n up to ~256 000 bits */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, but safe */

    /* Starting with 2^n - 1 */
    static uint64_t big[MAXWORDS];
    size_t words;
    big_init_from_2pow_minus1(big, &words, n);

    seq_t steps = 0;
    while (!big_is_one(big, words)) {
        unsigned tz = big_trailing_zeros(big, words);
        if (tz) {                     /* even number */
            big_shift_right(big, &words, tz);
            steps += tz;
            continue;
        }
        /* odd number: 3x+1 step */
        big_mult3(big, &words);
        big_add1(big, &words);
        steps += 1;                   /* count the 3x+1 step */
        unsigned tz2 = big_trailing_zeros(big, words);
        if (tz2) {
            big_shift_right(big, &words, tz2);
            steps += tz2;
        }
    }
    return steps;
}
