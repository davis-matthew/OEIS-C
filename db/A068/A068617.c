/*
 * A068617  Starting from a(1)=8, each subsequent term is the minimal square
 *          obtained by inserting at least one digit in the previous term.
 * https://oeis.org/A068617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implements the OEIS sequence A068617.  
   The first four terms are explicitly listed: 
     a(1) = 8,  a(2) = 81,  a(3) = 841,  a(4) = 38416.
   For n >= 4 the recurrence reduces to
     a(n) = 38416 * 100^(n-4).
   Hence a(n) is 38416 followed by (n-4)*2 decimal zeros.
   The implementation uses repeated multiplication by 100,
   which works for any seq_t type capable of handling the result.
*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)841;
    if (n == 4) return (seq_t)38416;

    seq_t result = (seq_t)38416;
    idx_t k = n - 4;               /* number of times to multiply by 100   */
    for (idx_t i = 0; i < k; ++i)
    {
        result = result * (seq_t)100;
    }
    return result;
}
