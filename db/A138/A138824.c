/*
 * A138824  Divisors of 4064 (the 4th perfect number divided by 2), written in
 *          base 2.
 * https://oeis.org/A138824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A138824 – the divisors of 4064 written in base 2 (decimal interpretation) */
/* a(1)=1, a(2)=10, a(3)=100, …, a(12)=111111100000 */
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    /* Determine the divisor d of 4064 corresponding to index n */
    seq_t d;
    if (n <= 6)               /* 1,2,4,8,16,32 */
        d = (seq_t)1 << (unsigned)(n - 1);
    else if (n <= 12)         /* 127,254,508,1016,2032,4064 */
        d = (seq_t)127 << (unsigned)(n - 7);
    else                      /* no further terms in the sequence */
        return 0;

    /* Convert d's binary representation to a decimal‑digit number */
    seq_t result = 0;
    unsigned len = bitlen_seq_t(d);          /* number of bits in d */

    for (unsigned i = 0; i < len; ++i) {
        unsigned pos   = len - 1 - i;         /* index of current bit from MSB */
        seq_t bit = (d >> pos) & 1;
        result = result * 10 + bit;
    }
    return result;
}
