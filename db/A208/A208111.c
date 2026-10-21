/*
 * A208111  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208111
 * Number of 6×n 0/1 arrays avoiding the patterns
 * horizontally 000 and 010 and vertically 011 and 110.
 *
 * Computation: linear recurrence of order 9
 * (obtained from the OEIS entry):
 *   a(n) = 2*a(n-1) + 21*a(n-3) + 438*a(n-4)
 *          - 456*a(n-5) - 399*a(n-6) - 7220*a(n-8)
 *          + 6859*a(n-9)
 *
 * Initial values (n = 1..9):
 *   a(1)=22, a(2)=484, a(3)=990, a(4)=2025,
 *   a(5)=22995, a(6)=261121, a(7)=768544,
 *   a(8)=2262016, a(9)=18608992.
 *
 * The function is memoised implicitly by only using a sliding
 * buffer of the last 9 terms, making it O(n) with a very small
 * constant factor and suitable for n up to 10 000.
 *
 * All arithmetic follows the rules for seq_t: use only +, -, *, /,
 * %, comparisons and integer constants. The code compiles both as
 * C11 and as C++17.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below offset – undefined, return 0 */

    /* Initial nine terms */
    static const seq_t init[9] = {
        (seq_t)22,     /* a(1) */
        (seq_t)484,    /* a(2) */
        (seq_t)990,    /* a(3) */
        (seq_t)2025,   /* a(4) */
        (seq_t)22995,  /* a(5) */
        (seq_t)261121, /* a(6) */
        (seq_t)768544, /* a(7) */
        (seq_t)2262016,/* a(8) */
        (seq_t)18608992/* a(9) */
    };

    if (n <= 9) return init[n-1];

    /* sliding window holding the last 9 terms:
     * buf[0] = a(k-8), buf[1] = a(k-7), ..., buf[8] = a(k)  */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) buf[i] = init[i];

    idx_t k;
    for (k = 10; k <= n; ++k) {
        /* recurrence computation */
        seq_t val = (seq_t)(2) * buf[8];
        val += (seq_t)(21) * buf[6];
        val += (seq_t)(438) * buf[5];
        val -= (seq_t)(456) * buf[4];
        val -= (seq_t)(399) * buf[3];
        val -= (seq_t)(7220) * buf[1];
        val += (seq_t)(6859) * buf[0];

        /* shift buffer left (discard oldest) and insert new value */
        for (int i = 0; i < 8; ++i) buf[i] = buf[i+1];
        buf[8] = val;
    }

    return buf[8]; /* a(n) */
}
