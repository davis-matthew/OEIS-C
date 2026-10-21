/*
 * A239820  Number of 2Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A239820:
   a(n) = 9*a(n-1) -28*a(n-2) +74*a(n-3) -181*a(n-4) +236*a(n-5)
      -261*a(n-6) +234*a(n-7) +200*a(n-8) -279*a(n-9) +104*a(n-10)
      -268*a(n-11) -36*a(n-12) +156*a(n-13) +104*a(n-14)
      -32*a(n-15) -16*a(n-16)   for n > 16
   Initial values:
      a(1)=5, a(2)=23, a(3)=132, a(4)=729, a(5)=3951, a(6)=21602,
      a(7)=118253, a(8)=646306, a(9)=3532732, a(10)=19315270,
      a(11)=105601487, a(12)=577330665, a(13)=3156353205,
      a(14)=17256335056, a(15)=94343160597, a(16)=515789128277.
*/

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamically‑grown cache of terms */
    static size_t cap   = 0;      /* allocated capacity of cache     */
    static size_t len   = 0;      /* number of terms actually stored */

    if (n == 0) return 0; /* undefined, but keep graceful */

    if ((size_t)n > len) {
        /* Ensure the array can hold at least n elements */
        ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

        /* Fill missing terms up to term n */
        for (idx_t i = len; i < n; ++i) {          /* i : 0‑based index of a(i+1) */
            if (i < 16) {                          /* initial terms (a(1)…a(16)) */
                static const seq_t init[16] = {
                    5ULL, 23ULL, 132ULL, 729ULL, 3951ULL,
                    21602ULL, 118253ULL, 646306ULL,
                    3532732ULL, 19315270ULL, 105601487ULL,
                    577330665ULL, 3156353205ULL, 17256335056ULL,
                    94343160597ULL, 515789128277ULL
                };
                cache[i] = init[i];
            } else {                               /* recurrence for i >= 16 */
                /* indices for previous terms */
                seq_t p1  = cache[i-1];    /* a(n-1)   */
                seq_t p2  = cache[i-2];    /* a(n-2)   */
                seq_t p3  = cache[i-3];    /* a(n-3)   */
                seq_t p4  = cache[i-4];    /* a(n-4)   */
                seq_t p5  = cache[i-5];    /* a(n-5)   */
                seq_t p6  = cache[i-6];    /* a(n-6)   */
                seq_t p7  = cache[i-7];    /* a(n-7)   */
                seq_t p8  = cache[i-8];    /* a(n-8)   */
                seq_t p9  = cache[i-9];    /* a(n-9)   */
                seq_t p10 = cache[i-10];   /* a(n-10)  */
                seq_t p11 = cache[i-11];   /* a(n-11)  */
                seq_t p12 = cache[i-12];   /* a(n-12)  */
                seq_t p13 = cache[i-13];   /* a(n-13)  */
                seq_t p14 = cache[i-14];   /* a(n-14)  */
                seq_t p15 = cache[i-15];   /* a(n-15)  */
                seq_t p16 = cache[i-16];   /* a(n-16)  */

                /* Separate positive and negative contributions to avoid
                   unsigned underflow. */
                seq_t pos = 0, neg = 0;

                pos += (seq_t)9   * p1;
                pos += (seq_t)74  * p3;
                pos += (seq_t)236 * p5;
                pos += (seq_t)234 * p7;
                pos += (seq_t)200 * p8;
                pos += (seq_t)104 * p10;
                pos += (seq_t)156 * p13;
                pos += (seq_t)104 * p14;

                neg += (seq_t)28   * p2;
                neg += (seq_t)181  * p4;
                neg += (seq_t)261  * p6;
                neg += (seq_t)279  * p9;
                neg += (seq_t)268  * p11;
                neg += (seq_t)36   * p12;
                neg += (seq_t)32   * p15;
                neg += (seq_t)16   * p16;

                cache[i] = pos - neg;   /* final term a(n) */
            }
        }
        len = (size_t)n;
    }

    return cache[n-1];
}
