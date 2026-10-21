/*
 * A070285  a(n) = n^(n-2) * (n-1)^(n-1).
 * https://oeis.org/A070285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) * (n-1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   mul_seq, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t.h"
#include "../utils/mul_seq.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 2. For n < 2 return 0 (undefined). */
    if (n < 2) return (seq_t)0;

    /* Compute n^(n-2) and (n-1)^(n-1) using the provided power routine */
    seq_t pow_n  = pow_seq_t((seq_t)n,     n-2);  /* n^(n-2) */
    seq_t pow_n1 = pow_seq_t((seq_t)(n-1), n-1);  /* (n-1)^(n-1) */

    /* Multiply the two results exactly */
    return mul_seq(pow_n, pow_n1);
}
