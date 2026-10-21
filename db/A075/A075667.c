/*
 * A075667  Sum of next n 6th powers.
 * https://oeis.org/A075667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21*n^13 + 231*n^11 + 693*n^9 + 549*n^7 - 126*n^5 -
 *            56*n^3 + 32*n)/1344
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 41, u128 n <= 760,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, n==0 is never used */

    seq_t nseq = (seq_t)n;

    /* For 64‑bit (or smaller) builtins use 128‑bit intermediates to avoid overflow.
       For arbitrary‑precision types we rely on their own unlimited arithmetic. */
    if (sizeof(seq_t) <= 8) {
        __int128 n128 = (__int128)nseq;

        __int128 n2  = n128 * n128;                 /* n^2 */
        __int128 n3  = n2  * n128;                 /* n^3 */
        __int128 n5  = n3  * n2;                   /* n^5 */
        __int128 n7  = n5  * n2;                   /* n^7 */
        __int128 n9  = n7  * n2;                   /* n^9 */
        __int128 n11 = n9  * n2;                   /* n^11 */
        __int128 n13 = n11 * n2;                   /* n^13 */

        __int128 num = 21 * n13 + 231 * n11 + 693 * n9 + 549 * n7;
        num = num - 126 * n5 - 56 * n3 + 32 * n128;

        return (seq_t)(num / 1344);
    } else {
        seq_t n2  = nseq * nseq;
        seq_t n3  = n2 * nseq;
        seq_t n5  = n3 * n2;
        seq_t n7  = n5 * n2;
        seq_t n9  = n7 * n2;
        seq_t n11 = n9 * n2;
        seq_t n13 = n11 * n2;

        seq_t num = 21 * n13 + 231 * n11 + 693 * n9 + 549 * n7;
        num = num - 126 * n5 - 56 * n3 + 32 * nseq;

        return num / 1344;
    }
}
