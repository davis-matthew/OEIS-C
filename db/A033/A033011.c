/*
 * A033011  Every run of digits of n in base 13 has length 2.
 * https://oeis.org/A033011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 14*B(n), where B(n) is the n-th number in base‑169 whose
 *            * digits are in 0..12, the most significant digit is non‑zero,
 *            and no two * consecutive digits are equal. The sequence a(n)
 *            lists the decimal values * of the corresponding base‑13 numbers
 *            whose digit runs all have length 2
 * Range:     u8 n <= 12, u16 n <= 156, u32 n <= 1800, u64 n <= 1800,
 *            u128 n <= 1800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0, return 0 for safety */

    /* Find the length k (number of base‑169 digits) of the n‑th number. */
    idx_t cumulative = 0;   /* sum of 12^1 … 12^{k-1} */
    idx_t pow12 = 12;       /* 12^1 */
    idx_t k = 1;            /* current length */

    while (cumulative + pow12 < n) {
        cumulative += pow12;     /* add count of length k */
        ++k;                     /* next length will be k+1 */
        pow12 *= 12;             /* pow12 = 12^k */
    }

    /* Offset within the block of length k (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Convert offset to base‑12 indices (digits from 0 to 11). */
    uint8_t idx_arr[64];            /* safe for all practical n */
    for (idx_t j = k; j > 0; --j) {
        idx_arr[j - 1] = offset % 12;
        offset /= 12;
    }

    /* Reconstruct the actual base‑169 digits with the run‑length‑2 constraint. */
    int prev_digit = (int)idx_arr[0] + 1;          /* first digit: 1..12 */
    seq_t B = prev_digit;                         /* base‑169 value */

    for (idx_t j = 1; j < k; ++j) {
        int idx = (int)idx_arr[j];
        int digit = (idx < prev_digit) ? idx : idx + 1;   /* skip previous digit */
        B = B * ((seq_t)169) + digit;
        prev_digit = digit;
    }

    /* Final sequence value: multiply by 14. */
    return B * ((seq_t)14);
}
