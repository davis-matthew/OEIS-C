/*
 * A173044  Product plus sum of five consecutive nonnegative numbers.
 * https://oeis.org/A173044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4) + n + (n+1) + (n+2) + (n+3) +
 *            (n+4)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast the index once into the arbitrary‑precision type */
    seq_t n_seq = (seq_t)n;

    /* Compute the product n*(n+1)*(n+2)*(n+3)*(n+4) */
    seq_t prod = n_seq;
    prod *= (n_seq + 1);
    prod *= (n_seq + 2);
    prod *= (n_seq + 3);
    prod *= (n_seq + 4);

    /* Add the sum of the five consecutive numbers */
    prod += n_seq;
    prod += (n_seq + 1);
    prod += (n_seq + 2);
    prod += (n_seq + 3);
    prod += (n_seq + 4);

    return prod;
}
