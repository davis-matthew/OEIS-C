/*
 * A174210  In the sequence of nonnegative natural numbers delete every 2nd,
 *          4th, 6th, 8th... occurrence of each individual digit 0, 1, 2,..,
 *          9.
 * https://oeis.org/A174210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A174210: delete even occurrences of each digit from the
   decimal representation of natural numbers in order. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* counts of each decimal digit seen so far (by total occurrence) */
    uint64_t counts[10] = {0};

    idx_t term_index = 0;    /* index of the term just produced */
    idx_t k = 0;             /* next natural number to process */

    while (1) {
        /* collect decimal digits of k, least significant first */
        idx_t tmp = k;
        uint8_t digs[20];
        int dlen = 0;
        if (tmp == 0) {
            digs[dlen++] = 0;
        } else {
            while (tmp != 0) {
                digs[dlen++] = tmp % 10;
                tmp /= 10;
            }
        }

        /* build the transformed value by deleting even‑occurrence digits */
        seq_t val = 0;
        bool has_digit = false;
        for (int i = dlen - 1; i >= 0; --i) {
            int d = digs[i];
            counts[d]++;                     /* this occurrence becomes counts[d] */
            if ((counts[d] & 1) == 0)        /* even occurrence – delete */
                continue;
            val = val * ((seq_t)10) + (seq_t)d;
            has_digit = true;
        }

        if (has_digit) {
            if (term_index == n)
                return val;
            term_index++;
        }

        k++; /* next natural number */
    }
}
