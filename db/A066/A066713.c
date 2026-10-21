/*
 * A066713  RATS(2^n): Reverse Add the digits of 2^n, Then Sort: a(n) =
 *          A036839(2^n).
 * https://oeis.org/A066713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sort_digits_nondecreasing( 2^n + reverse(2^n) )
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 33, u64 n <= 66, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_DIGITS 5000      /* enough for 2^10000 (≈3011 digits) */

/* Computes the n‑th term of OEIS A066713.
   2^n is represented in base‑10 as an array of digits (least
   significant first).  The array is then added to its reverse,
   the digits of the sum are sorted, and the sorted digits are
   packed into the result. */
seq_t a(idx_t n)
{
    unsigned char powd[MAX_DIGITS];       /* digits of 2^n, LSD first      */
    unsigned char sumd[MAX_DIGITS + 1];   /* digits of the sum             */
    int len = 1;                          /* length of powd                */
    powd[0] = 1;                          /* 2^0 = 1                       */

    /* Compute 2^n digit by digit */
    for (idx_t i = 0; i < n; ++i) {
        int carry = 0;
        for (int j = 0; j < len; ++j) {
            int tmp = powd[j] * 2 + carry;
            powd[j] = (unsigned char)(tmp % 10);
            carry   = tmp / 10;
        }
        while (carry) {
            powd[len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }
    }

    /* Add 2^n to its reverse */
    int sumlen = 0;
    int carry = 0;
    for (int i = 0; i < len; ++i) {
        int d1 = powd[i];
        int d2 = powd[len - 1 - i];
        int s  = d1 + d2 + carry;
        sumd[sumlen++] = (unsigned char)(s % 10);
        carry = s / 10;
    }
    while (carry) {
        sumd[sumlen++] = (unsigned char)(carry % 10);
        carry /= 10;
    }

    /* Count the digits of the sum */
    int cnt[10] = {0};
    for (int i = 0; i < sumlen; ++i) {
        ++cnt[sumd[i]];
    }

    /* Build the sorted result (ascending order) */
    seq_t result = 0;
    for (int d = 0; d <= 9; ++d) {
        for (int k = 0; k < cnt[d]; ++k) {
            result = result * 10 + d;
        }
    }

    return result;
}
