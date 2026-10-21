/*
 * A066489  Binary expansion of n followed by its reverse complement.
 * https://oeis.org/A066489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = binary(n) concatenated with the reverse complement of
 *            binary(n) interpreted as a decimal integer. The function returns
 *            that decimal value
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 31, u64 n <= 1023,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – n≥1 only */

    /* Determine the number of bits in n (k ≥ 1). */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++k;
    }

    /* Build the decimal value by appending bits. */
    seq_t result = 0;                 /* decimal accumulator */

    /* Append the original bits of n: from most significant to least. */
    for (idx_t i = k; i > 0; --i) {
        seq_t bit = (n >> (i - 1)) & 1;
        result = result * 10 + bit;
    }

    /* Append the reverse complement: complement bits from LSB upwards. */
    for (idx_t i = 0; i < k; ++i) {
        seq_t bit = (n >> i) & 1;
        seq_t comp = 1 - bit;         /* complement of the current bit */
        result = result * 10 + comp;
    }

    return result;
}
