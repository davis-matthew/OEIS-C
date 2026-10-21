/*
 * A054709  Number of powers of 8 modulo n.
 * https://oeis.org/A054709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct residues of powers of 8 modulo n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_powers_of_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_powers_of_8.h"

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but guard for completeness */
        return 0;

    return count_powers_of_8(n);
}
