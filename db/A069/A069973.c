/*
 * A069973  (Sum of digits of n)^3 - (sum of digits of n^3).
 * https://oeis.org/A069973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (sum of decimal digits of n)^3 - sum of decimal digits of
 *            n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  // for standard integer types

/* Compute the sum of decimal digits of an uint64_t.
   The result is returned as a seq_t. */
static inline seq_t digit_sum_uint64(uint64_t x)
{
    seq_t sum = (seq_t)0;
    while (x != 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Compute the sum of decimal digits of a value of type seq_t. */
static inline seq_t digit_sum_seq_t(seq_t x)
{
    seq_t sum = (seq_t)0;
    const seq_t ten = (seq_t)10;
    while (x != (seq_t)0) {
        sum += x % ten;
        x /= ten;
    }
    return sum;
}

/* OEIS sequence A069973 */
seq_t a(idx_t n)
{
    /* sum of digits of n */
    seq_t sum_n = digit_sum_uint64(n);

    /* (sum of digits of n)^3 */
    seq_t sum_n_cubed = sum_n * sum_n * sum_n;

    /* n^3 as a seq_t value */
    seq_t n_seq = (seq_t)n;
    seq_t cube = n_seq * n_seq * n_seq;

    /* sum of digits of n^3 */
    seq_t sum_cube = digit_sum_seq_t(cube);

    return sum_n_cubed - sum_cube;
}
