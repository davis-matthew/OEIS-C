/*
 * A101345  a(n) = Knuth's Fibonacci (or circle) product "2 o n".
 * https://oeis.org/A101345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000201(n+1) + n - 2 where A000201(k) = floor(k*phi)
 *            and phi = (1+sqrt(5))/2 A000201(k) is computed as floor((k +
 *            floor(k*sqrt(5)))/2) by using integer square root:
 *            floor(k*sqrt(5)) = floor(sqrt(5*k*k))
 * Range:     u8 n <= 6, u16 n <= 2219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_phi_seq, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_phi_seq.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t kplus1 = (seq_t)n + 1;          /* n + 1  */
    seq_t phi = floor_phi_seq(kplus1);    /* A000201(n+1) */

    return 2 * phi + (seq_t)n - 2;        /* a(n) = 2*phi + n - 2 */
}
