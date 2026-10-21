/*
 * A222141  Number of nX5 0..4 arrays with no entry increasing mod 5 by 4
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 542*a(n-1) -45648*a(n-2)
 *            +1362782*a(n-3) -14182035*a(n-4) -10420216*a(n-5)
 *            +672452128*a(n-6) -1495546560*a(n-7) Initial values: a(1)=256,
 *            a(2)=114244, a(3)=50999956, a(4)=22767656980,
 *            a(5)=10164078082036, a(6)=4537511642094436,
 *            a(7)=2025664524518719732
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first 7 terms directly */
    if (n == 1) return (seq_t)256ULL;
    if (n == 2) return (seq_t)114244ULL;
    if (n == 3) return (seq_t)50999956ULL;
    if (n == 4) return (seq_t)22767656980ULL;
    if (n == 5) return (seq_t)10164078082036ULL;
    if (n == 6) return (seq_t)4537511642094436ULL;
    if (n == 7) return (seq_t)2025664524518719732ULL;

    /* Keep the last seven terms in a circular buffer.
       buf[0] holds a(k-7), buf[1] holds a(k-6), ..., buf[6] holds a(k-1). */
    seq_t buf[7];
    buf[0] = (seq_t)256ULL;               /* a(1)  */
    buf[1] = (seq_t)114244ULL;            /* a(2)  */
    buf[2] = (seq_t)50999956ULL;          /* a(3)  */
    buf[3] = (seq_t)22767656980ULL;       /* a(4)  */
    buf[4] = (seq_t)10164078082036ULL;    /* a(5)  */
    buf[5] = (seq_t)4537511642094436ULL;  /* a(6)  */
    buf[6] = (seq_t)2025664524518719732ULL;/* a(7)  */

    /* Compute terms from a(8) up to a(n). */
    for (idx_t k = 8; k <= n; ++k) {
        seq_t next =
            (seq_t)542  * buf[6]
          - (seq_t)45648 * buf[5]
          + (seq_t)1362782 * buf[4]
          - (seq_t)14182035 * buf[3]
          - (seq_t)10420216 * buf[2]
          + (seq_t)672452128 * buf[1]
          - (seq_t)1495546560 * buf[0];

        /* Shift buffer left by one. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = next;
    }

    return buf[6];
}
