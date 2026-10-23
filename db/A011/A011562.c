/*
 * A011562  Stirling numbers of second kind S2(13,n).
 * https://oeis.org/A011562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling numbers of the second kind S(13,n) Definition:
 *            S(0,0)=1, S(n,0)=0 (n>0), S(n,k)=k*S(n-1,k)+S(n-1,k-1) (1 ≤ k ≤
 *            n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache the 13 values after the first call. */
    static seq_t cache[14];
    static bool init = false;

    if (!init) {
        seq_t s[14] = {0};
        s[0] = 1;                        /* S(0,0) */

        for (idx_t i = 1; i <= 13; ++i) {
            for (idx_t j = i; j >= 1; --j) {
                /* s[j] holds S(i-1,j), s[j-1] holds S(i-1,j-1) */
                seq_t tmp = s[j] * (seq_t)j + s[j - 1];
                s[j] = tmp;
            }
            s[0] = 0;                     /* S(i,0)=0 for i>0 */
        }

        /* Copy the resulting row S(13,k) to the cache. */
        for (idx_t k = 0; k <= 13; ++k)
            cache[k] = s[k];

        init = true;
    }

    return (n <= 13) ? cache[n] : 0;
}
