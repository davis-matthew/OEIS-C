/*
 * A171635  Squares in lunar arithmetic in base 8 written in base 10.
 * https://oeis.org/A171635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A171635:
   a(n) = decimal value of the lunar square of n in base 8.
   In lunar arithmetic: addition is digit‑wise maximum,
   multiplication uses digit‑wise minimum and column‑wise maximum. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑8 digits (least significant digit first).  */
    /* The maximum number of base‑8 digits for a 64‑bit input is < 21,  */
    /* we reserve a bit of slack just in case.  */
    uint8_t d[64] = {0};
    size_t len_d = 0;
    if (n == 0) {
        len_d = 1;
        d[0] = 0;
    } else {
        idx_t temp = n;
        while (temp > 0) {
            d[len_d++] = (uint8_t)(temp % 8);
            temp /= 8;
        }
    }

    /* The lunar square has at most 2*len_d digits.  */
    uint8_t res[128] = {0};
    size_t len_res = 2 * len_d;          /* upper bound */
    for (size_t i = 0; i < len_d; ++i) {
        for (size_t j = 0; j < len_d; ++j) {
            /* Partial product (digit‑wise minimum)  */
            uint8_t prod = d[i] < d[j] ? d[i] : d[j];
            size_t pos = i + j;           /* column index */
            if (prod > res[pos]) {
                res[pos] = prod;          /* column addition (max)  */
            }
        }
    }

    /* Find the most significant non‑zero digit.  */
    size_t msd = 0;
    for (size_t i = len_res; i > 0; --i) {
        if (res[i - 1] != 0) {
            msd = i;
            break;
        }
    }
    if (msd == 0) {        /* should not happen, but keep 0 safe  */
        msd = 1;
    }

    /* Convert the base‑8 result to decimal (seq_t).  */
    seq_t value = 0;
    for (int i = (int)msd - 1; i >= 0; --i) {
        value = value * 8 + (seq_t)res[i];
    }

    return value;
}
