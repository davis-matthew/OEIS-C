/*
 * A157464  Number of distinct squares when inserting at most one digit in
 *          decimal representation of n.
 * https://oeis.org/A157464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = # of distinct squares obtainable by inserting at most one
 *            decimal digit into the decimal representation of n
 * Range:     u8 n <= 9, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, floor_sqrt, pow10, seq_mul, seq_one, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Length of n in decimal digits */
    int m = digits_of(n);           /* 0 -> 1 */
    int target_len = m + 1;          /* length of the padded square string */

    /* Compute upper bound: 10^target_len - 1 */
    seq_t pow10_val = pow10((idx_t)target_len);
    seq_t one = seq_one();
    seq_t bound = pow10_val - one;

    /* Convert n to string */
    char n_buf[128];
    size_t n_len;
    seq_to_str((seq_t)n, n_buf, &n_len);
    n_buf[n_len] = '\0';

    seq_t count = 0;
    idx_t i = 0;
    for (;;) {
        seq_t sq = seq_mul((seq_t)i, (seq_t)i);   /* i^2 */
        if (sq > bound) break;                    /* larger squares unnecessary */

        /* Convert square to string */
        char s_buf[128];
        size_t s_len;
        seq_to_str(sq, s_buf, &s_len);
        s_buf[s_len] = '\0';

        /* Pad with leading zeros to target_len */
        int pad = target_len - (int)s_len;
        if (pad < 0) pad = 0;                     /* safety, should not happen */

        char padded[128];
        size_t padded_len = pad + s_len;
        for (int k = 0; k < pad; ++k) padded[k] = '0';
        memcpy(padded + pad, s_buf, s_len);
        padded[padded_len] = '\0';

        /* Check if deleting exactly one character from padded gives n */
        bool matched = false;
        for (int del = 0; del < padded_len && !matched; ++del) {
            size_t p = 0, q = 0;
            while (p < padded_len && q < n_len) {
                if ((int)p == del) { ++p; continue; }  /* skip the deleted char */
                if (padded[p] != n_buf[q]) break;
                ++p; ++q;
            }
            if (q == n_len) matched = true;
        }

        if (matched) count++;

        ++i;
    }

    return count;
}
