/*
 * A105029  Write numbers in binary under each other, left justified, read
 *          diagonals in downward direction, convert to decimal.
 * https://oeis.org/A105029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer obtained by reading the n‑th down‑right *
 *            diagonal of the table whose rows are the binary representations
 *            * of 0,1,2,… written left‑justified, then interpreting the
 *            collected * bits as a binary number. Example: a(0)=0, a(1)=2,
 *            a(2)=6, a(3)=5,
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_idx, diag_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bitlen_idx.h"
#include "../utils/diag_value.h"

seq_t a(idx_t n)
{
    return diag_value(n);
}
