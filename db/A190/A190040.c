/*
 * A190040  Number of nondecreasing arrangements of n+2 numbers in 0..8 with
 *          the last equal to 8 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 50*n)/3 + 2*n^2 - 83 for n > 6, a(1..6) = 13, 24,
 *            40, 65, 105, 164 (given directly)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Pre‑computed initial values for n = 1..6 */
static const seq_t init[6] = {13, 24, 40, 65, 105, 164};

seq_t a(idx_t n)
{
    /* For n <= 6 use the supplied initial values */
    if (n <= 6)
        return init[n - 1];

    /* Compute the closed‑form expression for n > 6.
       All intermediate calculations use seq_t and rely on
       integer arithmetic only.  The final result is an
       integer because (n^3 + 50*n) is always divisible by 3. */
    const seq_t N  = (seq_t)n;         /* n as seq_t */
    const seq_t N2 = N * N;            /* n^2 */
    const seq_t N3 = N2 * N;           /* n^3 */

    /* (n^3 + 50*n)/3 + 2*n^2 - 83 */
    const seq_t term13 = (N3 + (seq_t)50 * N) / (seq_t)3;
    const seq_t result = term13 + (seq_t)2 * N2 - (seq_t)83;

    return result;
}
