/*
 * A220011  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A220011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/60)*n^5 - (1/4)*n^4 + (31/12)*n^3 - (35/4)*n^2 +
 *            (102/5)*n - 17 (exact for n > 2; for n = 1,2 use a(1)=4, a(2)=7)
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 194, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* offsets */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    /* For builtin integer seq_t types (<= 8 bytes) use 128‑bit arithmetic
       to avoid overflow during the intermediate calculations. */
    if (sizeof(seq_t) <= 8) {
        signed __int128 n128 = n;
        signed __int128 n2  = n128 * n128;
        signed __int128 n3  = n2  * n128;
        signed __int128 n4  = n3  * n128;
        signed __int128 n5  = n4  * n128;

        signed __int128 num = n5
            - (signed __int128)15 * n4
            + (signed __int128)155 * n3
            - (signed __int128)525 * n2
            + (signed __int128)1224 * n128
            - (signed __int128)1020;

        return (seq_t)(num / 60);
    }

    /* For arbitrary‑precision or larger builtin types (>= 16 bytes) use the
       same formula but compute the numerator as (pos - neg) to avoid
       negative intermediate values that could underflow an unsigned type. */
    {
        seq_t n128 = (seq_t)n;

        seq_t n2 = n128 * n128;
        seq_t n3 = n2  * n128;
        seq_t n4 = n3  * n128;
        seq_t n5 = n4  * n128;

        seq_t pos = n5 + (seq_t)155 * n3 + (seq_t)1224 * n128;
        seq_t neg = (seq_t)15 * n4 + (seq_t)525 * n2 + (seq_t)1020;

        seq_t num = pos - neg;   /* guaranteed non‑negative for n > 2 */

        return num / (seq_t)60;
    }
}
