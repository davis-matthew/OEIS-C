/*
 * A183657  Number of (n+1) X 5 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 150*5^n + 720*4^n + 1710*3^n + 2640*2^n + 2550
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer (big‑integer) exponentiation, binary exponentiation. */
static seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur    = base;

    while (exp != 0) {
        if (exp & 1u)         /* exp is at least 1 bit wide. */
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A183657 */
seq_t a(idx_t n)
{
    seq_t t6 = 6   * _pow_seq((seq_t)6, n);
    seq_t t5 = 150 * _pow_seq((seq_t)5, n);
    seq_t t4 = 720 * _pow_seq((seq_t)4, n);
    seq_t t3 = 1710 * _pow_seq((seq_t)3, n);
    seq_t t2 = 2640 * _pow_seq((seq_t)2, n);
    seq_t c  = (seq_t)2550;

    return t6 + t5 + t4 + t3 + t2 + c;
}
