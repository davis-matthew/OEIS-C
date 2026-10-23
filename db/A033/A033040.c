/*
 * A033040  Numbers all of whose base 15 digits are odd.
 * https://oeis.org/A033040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = nth number whose digits in base 15 are all odd.
   The sequence consists of all integers that can be written in base‑15
   using only the digits {1,3,5,7,9,11,13}.  The coding below finds the
   block of integers that have the same number of base‑15 digits and
   then maps the offset within that block to the corresponding
   base‑7 representation.  Each base‑7 digit d (0…6) is translated
   into the odd base‑15 digit odd_map[d].  Finally the decimal value
   is reconstructed by successive multiplication by 15.  All arithmetic
   that may overflow the index type is performed using idx_t (uint64_t);
   the value itself is accumulated in seq_t.  The function works for
   any n ≥ 1 and is order‑independent.  */
#include <stdbool.h>
#include <stdint.h>

static const unsigned char odd_map[7] = {1, 3, 5, 7, 9, 11, 13};

seq_t a(idx_t n)
{
    /* Determine length k of the base‑15 representation.
       cumulative counts numbers with fewer than k digits. */
    idx_t cumulative = 0;       /* numbers with shorter lengths */
    idx_t pow7 = 7;             /* 7^k for current k (k starts at 1) */
    idx_t k = 1;

    /* Find the smallest k such that cumulative + pow7 >= n.
       Safely handle potential integer overflow. */
    while (true) {
        /* If cumulative + pow7 would overflow, stop – the limit is reached. */
        if (pow7 > UINT64_MAX - cumulative)
            break;

        if (cumulative + pow7 >= n)
            break;

        cumulative += pow7;

        /* Prepare pow7 for next k, stop if next power would overflow. */
        if (pow7 > UINT64_MAX / 7)
            break;
        pow7 *= 7;
        ++k;
    }

    /* Offset within the k‑digit block (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Convert offset to base‑7 digits, little‑endian. */
    unsigned char digits[64]; /* enough for k up to ~64 */
    for (idx_t i = 0; i < k; ++i) {
        digits[i] = offset % 7;
        offset   /= 7;
    }

    /* Build the decimal value from the base‑15 digits. */
    seq_t val = 0;
    for (idx_t i = k; i-- > 0; ) {
        val = val * 15 + odd_map[digits[i]];
    }

    return val;
}
