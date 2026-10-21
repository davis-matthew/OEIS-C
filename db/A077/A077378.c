/*
 * A077378  Squarefree numbers whose external as well as internal digits form
 *          a squarefree number.
 * https://oeis.org/A077378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_squarefree, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A077378 by brute‑force search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    seq_t count = 0;          /* how many terms we have found so far */
    seq_t m = 100;            /* start testing from the first 3‑digit number */

    while (true) {
        /* Only squarefree numbers can be candidates */
        if (is_squarefree(m)) {
            int d = digit_len(m);        /* number of decimal digits */
            if (d >= 3) {                /* at least 3 digits → internal part exists */
                seq_t pow10_first = pow10((idx_t)(d - 1));
                seq_t first = m / pow10_first;
                seq_t internal = (m % pow10_first) / (seq_t)10;
                seq_t last = m % (seq_t)10;
                seq_t external = first * (seq_t)10 + last;

                /* 1 is treated as NOT squarefree, and 0 cannot be squarefree either */
                if (external != (seq_t)1 && internal != (seq_t)1 &&
                    external != (seq_t)0 && internal != (seq_t)0 &&
                    is_squarefree(external) && is_squarefree(internal)) {
                    count++;
                    if (count == n)
                        return m;
                }
            }
        }
        m++;      /* next integer to test */
    }
}
