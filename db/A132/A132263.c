/*
 * A132263  Product{0<=k<=floor(log_11(n)), floor(n/11^k)}, n>=1.
 * https://oeis.org/A132263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132263: Product{0<=k<=floor(log_11(n)), floor(n/11^k)} */
/*           a(n) = ∏_{k=0}^{⌊log₁₁ n⌋} ⌊n / 11ᵏ⌋,   n ≥ 1    */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* not defined for 0, return 0 to avoid UB */
        return 0;

    seq_t result = 1;          /* accumulate product */
    idx_t pow    = 1;          /* current power of 11, 1 = 11⁰ */

    for (;;)
    {
        seq_t term = (seq_t)(n / pow);   /* floor(n / 11ᵏ) */
        result *= term;                  /* multiply into product */

        /* stop when next power would exceed n (and 11ᵏ grows) */
        if (pow > n / 11)
            break;
        pow *= 11;
    }
    return result;
}
