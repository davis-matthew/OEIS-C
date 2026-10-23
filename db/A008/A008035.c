/*
 * A008035  Coordination sequence T2 for Zeolite Code APD.
 * https://oeis.org/A008035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=11, a(3)=22, a(4)=38, a(5)=59, a(6)=83,
 *            a(7)=113, a(8)=147, a(9)=186, a(10)=230, a(11)=278, a(12)=330,
 *            a(13)=386, a(14)=448, a(15)=513, a(16)=585, a(17)=660,
 *            a(18)=739, a(19)=824, a(20)=912, a(21)=1006. For n≥22 the
 *            sequence satisfies the linear recurrence derived from the de...
 * Range:     u8 n <= 10, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values up to n = 21 (inclusive) */
    static const seq_t init[22] = {
        1ULL, 4ULL, 11ULL, 22ULL, 38ULL, 59ULL, 83ULL, 113ULL, 147ULL,
        186ULL, 230ULL, 278ULL, 330ULL, 386ULL, 448ULL, 513ULL, 585ULL,
        660ULL, 739ULL, 824ULL, 912ULL, 1006ULL
    };

    if (n < 22)
        return init[n];

    /* Allocate an array big enough to hold all terms up to n */
    size_t len = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!buf)
        return 0;  /* allocation failure – unlikely for n ≤ 10⁴ */

    /* Copy the known initial terms */
    for (idx_t i = 0; i < 22; ++i)
        buf[i] = init[i];

    /* Compute successive terms using the recurrence */
    for (idx_t i = 22; i <= n; ++i)
    {
        seq_t plus  = buf[i-3] + buf[i-7] + buf[i-11] + buf[i-21];
        seq_t minus = buf[i-10] + buf[i-14] + buf[i-18];
        buf[i] = plus - minus;   /* the result is always non‑negative */
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
