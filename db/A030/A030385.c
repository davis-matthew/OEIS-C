/*
 * A030385  (# 2's)-(# 3's) in first n terms of s=A030373.
 * https://oeis.org/A030385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of 2's) – (# of 3's) in the first n terms of the
 *            sequence A030373 (base‑4 digits of the positive integers
 *            concatenated)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Memoization state */
    static seq_t *diff   = NULL;
    static size_t diff_cap  = 0;           /* allocated size of diff array   */
    static idx_t generated   = 0;          /* how many digits already processed */
    static uint64_t cur_num   = 1;         /* the next integer whose digits are visited */
    static unsigned char digits[64];       /* digits of cur_num (MSB first)   */
    static unsigned cur_digit_idx   = 0;   /* position within digits array     */
    static unsigned cur_num_digits = 0;   /* number of digits in cur_num      */

    /* Ensure diff has capacity for indices up to n. */
    if (n >= diff_cap) {
        size_t new_cap = diff_cap ? diff_cap * 2 : 16;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        diff = (seq_t *)realloc(diff, new_cap * sizeof(seq_t));
        diff_cap = new_cap;
        diff[0] = (seq_t)0;               /* a(0) is defined as 0 */
    }

    /* Lazily initialise the first integer's digits (cur_num == 1). */
    if (generated == 0) {
        uint64_t t = cur_num;
        unsigned rev[64];
        unsigned rcnt = 0;
        if (t == 0) {
            rev[rcnt++] = 0;
        } else {
            while (t > 0) {
                rev[rcnt++] = (unsigned)(t & 3); /* t mod 4  */
                t >>= 2;
            }
        }
        cur_num_digits = rcnt;
        for (unsigned i = 0; i < rcnt; ++i) {
            digits[i] = rev[rcnt - 1 - i];
        }
    }

    /* Generate and accumulate digits until we have a(n). */
    while (generated < n) {
        unsigned d = digits[cur_digit_idx];
        seq_t prev = diff[generated];
        if (d == 2) {
            diff[generated + 1] = prev + (seq_t)1;
        } else if (d == 3) {
            diff[generated + 1] = prev - (seq_t)1;
        } else {
            diff[generated + 1] = prev;     /* d == 0 or 1 */
        }

        ++generated;
        ++cur_digit_idx;

        /* When all digits of cur_num are consumed, move to the next integer. */
        if (cur_digit_idx >= cur_num_digits) {
            ++cur_num;
            uint64_t t = cur_num;
            unsigned rev[64];
            unsigned rcnt = 0;
            if (t == 0) {
                rev[rcnt++] = 0;
            } else {
                while (t > 0) {
                    rev[rcnt++] = (unsigned)(t & 3);
                    t >>= 2;
                }
            }
            cur_num_digits = rcnt;
            for (unsigned i = 0; i < rcnt; ++i) {
                digits[i] = rev[rcnt - 1 - i];
            }
            cur_digit_idx = 0;
        }
    }

    return diff[n];
}
