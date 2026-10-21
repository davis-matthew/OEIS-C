/*
 * A239016  Numbers not larger than any rotation of their digits.
 * https://oeis.org/A239016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239016: Numbers not larger than any rotation of their digits.
   Implementation: iteratively test each integer, accept it if no rotation
   yields a smaller number. Results are cached so that repeated calls are
   efficient. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static state for memoized list */
    static seq_t *seq   = NULL;   /* array of found terms      */
    static size_t seq_len = 0;    /* current number of terms   */
    static size_t seq_cap = 0;    /* allocated capacity        */
    static seq_t cur = 0;         /* next integer to test      */

    /* expand cache until it holds at least n terms */
    while (seq_len < (size_t)n) {
        bool ok = true;

        /* special case: 0 is a valid term */
        if (cur == 0) {
            ok = true;
        } else {
            /* length of decimal representation */
            size_t len = digit_len(cur);
            if (len > 1) {
                /* 10^len value needed for rotation computation */
                seq_t pow10_len = pow10(len);
                seq_t pow10k    = 10;        /* 10^1 initially */
                for (size_t k = 1; k < len; ++k) {
                    /* rotation obtained by moving the last k digits to front */
                    seq_t left  = cur / pow10k;
                    seq_t right = cur % pow10k;
                    seq_t rot   = right * (pow10_len / pow10k) + left;
                    if (rot < cur) {          /* found a smaller rotation */
                        ok = false;
                        break;
                    }
                    pow10k *= 10;              /* advance to next power of ten */
                }
            }
        }

        if (ok) {
            /* add cur to cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = cur;
        }
        ++cur;
    }

    return seq[n - 1];
}
