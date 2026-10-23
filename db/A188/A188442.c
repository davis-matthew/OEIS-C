/*
 * A188442  Expansion of -(sqrt(-3*x^2-2*x+1)-x-1)/(2*sqrt(-3*x^2-2*x+1)+2*x).
 * https://oeis.org/A188442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=3 a(n) satisfies the D‑finite
 *            recurrence n*(5*n-2)*a(n) + (-20*n^2+23*n-12)*a(n-1) +
 *            3*(-5*n^2+7*n+20)*a(n-2) + 2*(35*n^2-89*n-12)*a(n-3) +
 *            12*(5*n+3)*(n-4)*a(n-4) = 0 (for n≥4)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* For small n use the known initial values. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* Allocate an array that can hold all terms up to n. */
    size_t len = (size_t)n + 1;
    seq_t *buf = alloc_seq_array(len);
    if (!buf) abort();           /* allocation failure */

    /* Initialize the first four values. */
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)1;
    buf[3] = (seq_t)3;

    /* Compute the remaining terms using the recurrence. */
    for (size_t i = 4; i <= (size_t)n; ++i)
    {
        seq_t nn  = (seq_t)i;

        seq_t c1 = -(seq_t)(20)*nn*nn + (seq_t)(23)*nn - (seq_t)(12);
        seq_t c2 = (seq_t)(3)*((seq_t)(-5)*nn*nn + (seq_t)(7)*nn + (seq_t)(20));
        seq_t c3 = (seq_t)(2)*((seq_t)(35)*nn*nn - (seq_t)(89)*nn - (seq_t)(12));
        seq_t c4 = (seq_t)(12)*((seq_t)(5)*nn + (seq_t)(3))*(nn - (seq_t)(4));

        seq_t denom = nn * ((seq_t)(5)*nn - (seq_t)(2));

        seq_t sum = buf[i-1]*c1 + buf[i-2]*c2 + buf[i-3]*c3 + buf[i-4]*c4;
        buf[i] = -sum / denom;
    }

    seq_t result = buf[(size_t)n];
    free_seq_array(buf);
    return result;
}
