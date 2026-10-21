/*
 * A098944  Numbers where 8 is the only even decimal digit.
 * https://oeis.org/A098944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098944: numbers whose decimal representation
 * contains only the digits 1,3,5,7,8,9 and at least one 8.
 * Computed by combinatorial indexing: first determine the
 * length of the number, then walk through the digit positions
 * selecting the digit that contains the required offset. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 6 and 5 up to the needed exponent. */
    static seq_t powers6[65] = {0};
    static seq_t powers5[65] = {0};
    static int   max_precomp = 0;

    if (max_precomp == 0) {
        powers6[0] = (seq_t)1;
        powers5[0] = (seq_t)1;
    }

    /* Find the digit length containing the n-th number. */
    idx_t  length = 1;
    seq_t  six_pow = (seq_t)6;   /* 6^1 */
    seq_t  five_pow = (seq_t)5;  /* 5^1 */
    idx_t  total   = 0;          /* count of numbers of shorter lengths */

    while (true) {
        seq_t count_with8 = six_pow - five_pow;

        if ((total + (idx_t)count_with8) >= n)
            break;

        total += (idx_t)count_with8;
        ++length;
        six_pow *= (seq_t)6;
        five_pow *= (seq_t)5;
    }

    /* offset inside the block of numbers with the current length (0‑based) */
    idx_t offset = n - total - 1;

    /* Make sure power tables contain the needed exponents. */
    if (max_precomp < (int)length) {
        for (int i = max_precomp + 1; i <= (int)length; ++i) {
            powers6[i] = powers6[i - 1] * (seq_t)6;
            powers5[i] = powers5[i - 1] * (seq_t)5;
        }
        max_precomp = (int)length;
    }

    /* Digit list in ascending order. */
    static const int digits[6] = {1, 3, 5, 7, 8, 9};

    seq_t  result = (seq_t)0;
    bool   has8   = false;

    for (idx_t pos = 0; pos < length; ++pos) {
        int remaining = (int)(length - pos - 1);

        for (int k = 0; k < 6; ++k) {
            int    d          = digits[k];
            bool   next_has8  = has8 || (d == 8);
            seq_t  count;
            if (remaining == 0) {
                /* At the last position we can choose this digit only
                 * if it guarantees at least one 8 somewhere. */
                count = next_has8 ? (seq_t)1 : (seq_t)0;
            } else if (next_has8) {
                count = powers6[remaining];
            } else {
                count = powers6[remaining] - powers5[remaining];
            }

            idx_t count_idx = (idx_t)count;
            if (offset < count_idx) {
                result = result * (seq_t)10 + (seq_t)d;
                has8    = next_has8;
                break;
            } else {
                offset -= count_idx;
            }
        }
    }

    return result;
}
