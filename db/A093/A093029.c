/*
 * A093029  Indices of terms in A093019 with value 9.
 * https://oeis.org/A093029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = smallest integer k ≥ 1 such that the Luhn check digit of
 *            k equals 9, and count the first n such integers. (Offset 1)
 * Range:     u8 n <= 26, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Luhn check digit of a non‑negative integer given in type seq_t.
   The function returns an int in 0…9. */
static int luhn_check_digit(seq_t v)
{
    seq_t sum = 0;
    int weight = 2;           /* rightmost digit is doubled (weight 2) */

    while (v > 0) {
        int d = (int)(v % 10);
        v /= 10;

        if (weight == 2) {
            d = d * 2;
            if (d > 9) d -= 9;   /* subtract 9 for numbers > 9 */
        }

        sum += d;
        weight ^= 3;           /* toggle weight between 2 and 1 (2^1 = 3) */
    }

    int rem = (int)(sum % 10);
    return (10 - rem) % 10;     /* check digit that makes the whole sum a multiple of 10 */
}

seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of found values so far */
    seq_t candidate = 1;        /* candidate integer to test */

    while (1) {
        if (luhn_check_digit(candidate) == 9) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
}
