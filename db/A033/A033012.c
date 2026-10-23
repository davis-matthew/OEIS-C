/*
 * A033012  Every run of digits of n in base 14 has length 2.
 * https://oeis.org/A033012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 15*A043318(n)
 * Range:     u8 n <= 13, u16 n <= 182, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused, offset starts at 1 */

    /* Find the length L of the base‑196 representation of the
     * n‑th number whose digits are in {0,…,13} and adjacent digits differ.
     * The count of such numbers of length L is 13^L.
     */
    idx_t pow13 = 13;      /* 13^1 */
    idx_t total = 13;      /* total numbers up to length 1 */
    int   L = 1;
    while (n > total) {
        ++L;
        pow13 = pow13 * 13;   /* 13^L */
        total += pow13;       /* cumulative count */
    }

    /* offset of n within the block of numbers having length L (1‑based) */
    idx_t prev_total = total - pow13;          /* count before this block  */
    idx_t offset     = n - prev_total;         /* 1‑based within block   */
    idx_t remaining  = offset - 1;             /* 0‑based index          */

    /* Most significant digit (cannot be zero) */
    int digits[20];
    int msd_choice = (int)(remaining / (pow13 / 13));   /* 0..12 */
    digits[0] = 1 + msd_choice;                          /* 1..13 */
    remaining %= (pow13 / 13);

    /* Pre‑compute powers of 13 for suffix counting */
    idx_t pow13k[20];
    pow13k[0] = 1;
    for (int i = 1; i < L; ++i) pow13k[i] = pow13k[i - 1] * 13ULL;

    /* Determine the following digits */
    for (int pos = 1; pos < L; ++pos) {
        int rem_pos = L - pos;                       /* remaining positions including this one */
        idx_t per_choice = pow13k[rem_pos - 1];       /* number of completions after choosing this digit */

        int digit_choice = (int)(remaining / per_choice);
        remaining %= per_choice;

        /* Find the digit_choice‑th digit that differs from the previous one */
        int d = 0;
        int count = 0;
        while (1) {
            if (d == digits[pos - 1]) {              /* skip equal digit */
                ++d;
                continue;
            }
            if (count == digit_choice) break;
            ++d;
            ++count;
        }
        digits[pos] = d;
    }

    /* Convert the base‑196 digits to the value y = Σ b_k * 196^k  */
    seq_t y = 0;
    seq_t pow196 = 1;                                 /* 196^0 */
    for (int i = L - 1; i >= 0; --i) {
        y += (seq_t)digits[i] * pow196;
        pow196 *= 196;                                /* next power */
    }

    /* Final result a(n) = 15 * y */
    return (seq_t)15 * y;
}
