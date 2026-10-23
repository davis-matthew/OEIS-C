/*
 * A031219  Write n in base 5 and juxtapose.
 * https://oeis.org/A031219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   a(n) = digit at position n of the Champernowne constant in base
 *            5
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n >= 1 by definition.  Find the block of base‑5 numbers containing
       the n-th digit.  Base‑5 numbers of length k count as 4·5^(k‑1) numbers,
       each contributing k digits.  */
    unsigned long long k = 1;                 /* current block length */
    unsigned long long cumulative = 0;        /* digits before current block */
    unsigned long long pow5k_minus1 = 1;       /* 5^(k‑1) */

    while (1) {
        unsigned long long num_count = 4ULL * pow5k_minus1;          /* # of numbers of length k */
        unsigned long long digits_in_block = k * num_count;         /* digits contributed by block */
        if (cumulative + digits_in_block >= n) break;              /* n lies in this block */
        cumulative += digits_in_block;
        k++;
        pow5k_minus1 *= 5ULL;                                      /* advance 5^(k‑1) */
    }

    /* Position inside the block (0‑based) */
    unsigned long long offset = n - cumulative - 1;                /* n <= cumulative+digits_in_block */
    unsigned long long num_index   = offset / k;                   /* index of the number within the block */
    unsigned long long digit_index = offset % k;                   /* digit position inside that number */

    /* The actual decimal value of the number is 5^(k‑1) + num_index */
    unsigned long long value = pow5k_minus1 + num_index;

    /* Compute 5^(k‑1‑digit_index) by repeated division */
    unsigned long long pow_needed = pow5k_minus1;
    for (unsigned long long i = 0; i < digit_index; ++i) {
        pow_needed /= 5ULL;
    }

    unsigned long long digit = (value / pow_needed) % 5ULL;
    return (seq_t)digit;
}
