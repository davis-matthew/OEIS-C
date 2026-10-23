/*
 * A161666  Smallest of 4 consecutive composite numbers which sum up to prime.
 * https://oeis.org/A161666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 351, u32 n <= 447, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A161666.
   It scans the natural numbers, collects consecutive composite
   numbers in the sequence of composites, and records the smallest
   of every block of four composites whose sum is prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"        /* is_prime(idx_t)      */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t)  */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range - not used */

    idx_t count = 0;   /* how many terms we have found so far */
    idx_t i = 2;       /* current integer to test       */
    idx_t window[4];   /* holds the last four composites */
    int wsize = 0;     /* number of composites currently in window */

    seq_t result = 0;  /* the n‑th term when found */

    while (count < n) {
        if (!is_prime(i)) {           /* i is composite */
            if (wsize < 4) {
                window[wsize++] = i;   /* fill window */
            } else {
                /* slide window left and add new composite */
                window[0] = window[1];
                window[1] = window[2];
                window[2] = window[3];
                window[3] = i;
            }

            if (wsize == 4) {
                /* sum of the four composites in terms of seq_t */
                seq_t sum = (seq_t)window[0]
                          + (seq_t)window[1]
                          + (seq_t)window[2]
                          + (seq_t)window[3];

                if (is_prime_big(sum)) {
                    result = window[0]; /* smallest of the block */
                    count++;
                }
            }
        }
        i++;
    }
    return result;
}
