/*
 * A209548  1/4 the number of (n+1)X4 0..3 arrays with every 2X2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209548
   Sequence: a(n) = 15*a(n-1) -59*a(n-2) +27*a(n-3) +267*a(n-4) -488*a(n-5)
               +111*a(n-6) +312*a(n-7) -174*a(n-8) -30*a(n-9) +18*a(n-10)
               +2*a(n-11)
   initial: a(1)=61, a(2)=562, a(3)=5075, a(4)=46515, a(5)=425803,
            a(6)=3901194, a(7)=35741496, a(8)=327471411, a(9)=3000373035,
            a(10)=27490276714, a(11)=251873944521
*/
#include <stdbool.h>
#include <stdint.h>

static const seq_t init_seq[11] = {
    (seq_t)61,          /* a(1) */
    (seq_t)562,         /* a(2) */
    (seq_t)5075,        /* a(3) */
    (seq_t)46515,       /* a(4) */
    (seq_t)425803,      /* a(5) */
    (seq_t)3901194,     /* a(6) */
    (seq_t)35741496,    /* a(7) */
    (seq_t)327471411,   /* a(8) */
    (seq_t)3000373035,  /* a(9) */
    (seq_t)27490276714, /* a(10) */
    (seq_t)251873944521 /* a(11) */
};

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 as a placeholder */
    if (n == 0) {
        return (seq_t)0;
    }
    if (n <= 11) {
        return init_seq[n - 1];
    }

    /* Roll‑around buffer holding the last 11 terms:
       buf[10] -> a(k-1), buf[9] -> a(k-2), ..., buf[0] -> a(k-11)   */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i) {
        buf[i] = init_seq[i];
    }

    for (idx_t k = 12; k <= n; ++k) {
        /* Compute a(k) from the previous 11 terms */
        seq_t val = (seq_t)15 * buf[10];
        val   -= (seq_t)59 * buf[9];
        val   += (seq_t)27 * buf[8];
        val   += (seq_t)267 * buf[7];
        val   -= (seq_t)488 * buf[6];
        val   += (seq_t)111 * buf[5];
        val   += (seq_t)312 * buf[4];
        val   -= (seq_t)174 * buf[3];
        val   -= (seq_t)30  * buf[2];
        val   += (seq_t)18  * buf[1];
        val   += (seq_t)2   * buf[0];

        /* Shift buffer left by one position */
        for (idx_t i = 0; i < 10; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[10] = val;  /* Now buf[10] holds a(k) */
    }

    return buf[10];
}
