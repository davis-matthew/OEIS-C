/*
 * A235306  Number of (n+1) X (4+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A235306
    Recurrence:
    a(n) = 4*a(n-1) +25*a(n-2) –112*a(n-3) –231*a(n-4) +1260*a(n-5)
           +875*a(n-6) –7280*a(n-7) –224*a(n-8) +22736*a(n-9)
           –7980*a(n-10) –36288*a(n-11) +21456*a(n-12)
           +23040*a(n-13) –17280*a(n-14)
    with a(1)=1554, a(2)=2918, … , a(14)=80708774
    Offset: 1
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* offset is 1; undefined for n=0 */
        return (seq_t)0;
    }

    /* Initial 14 values */
    static const seq_t init[14] = {
        (seq_t)1554,
        (seq_t)2918,
        (seq_t)5042,
        (seq_t)10790,
        (seq_t)21090,
        (seq_t)49574,
        (seq_t)107186,
        (seq_t)269222,
        (seq_t)629154,
        (seq_t)1656998,
        (seq_t)4101362,
        (seq_t)11181350,
        (seq_t)28857570,
        (seq_t)80708774
    };

    if (n <= 14) {
        return init[(size_t)n - 1];
    }

    /* Buffer of the last 14 computed terms:
       buf[0] = a(k-13), buf[13] = a(k) at each iteration.
    */
    seq_t buf[14];
    for (size_t i = 0; i < 14; ++i) {
        buf[i] = init[i];
    }

    /* Compute successive terms up to a(n) */
    for (size_t k = 14; k < (size_t)n; ++k) {
        seq_t pos = 0, neg = 0;
        /* Positives: a(k) uses previous 14 terms in order buf[13]..buf[0] */
        pos += (seq_t)4   * buf[13];
        pos += (seq_t)25  * buf[12];
        neg += (seq_t)112 * buf[11];
        neg += (seq_t)231 * buf[10];
        pos += (seq_t)1260* buf[9];
        pos += (seq_t)875 * buf[8];
        neg += (seq_t)7280* buf[7];
        neg += (seq_t)224 * buf[6];
        pos += (seq_t)22736* buf[5];
        neg += (seq_t)7980* buf[4];
        neg += (seq_t)36288* buf[3];
        pos += (seq_t)21456* buf[2];
        pos += (seq_t)23040* buf[1];
        neg += (seq_t)17280* buf[0];

        /* New value a(k+1) */
        seq_t next = pos - neg;

        /* Shift buffer left so that buf[13] is the new value */
        for (size_t i = 0; i < 13; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[13] = next;
    }

    return buf[13]; /* a(n) is currently stored in buf[13] */
}
