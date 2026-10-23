/*
 * A135586  a(1)=0; for n >= 1, a(2n)=a(n)+2^A000120(n)-1, a(2n+1)=2a(2n).
 * https://oeis.org/A135586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=0; a(2n)=a(n)+2^popcount(n)-1; a(2n+1)=2*a(2n);
 * Range:     u8 n <= 62, u16 n <= 8190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    idx_t prefix = 1;          /* current prefix of the binary representation */
    seq_t val = (seq_t)0;      /* accumulated a(prefix) */

    /* position of the most significant set bit (0‑based, n≥1) */
    int msb = 63 - __builtin_clzll(n);

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);
        int pc   = __builtin_popcountll(prefix);
        seq_t pow2 = (seq_t)(1ULL << pc); /* 2^popcount(prefix) */

        if (bit == 0) {           /* even case: n = 2*prefix */
            val = val + pow2 - (seq_t)1;
            prefix <<= 1;
        } else {                  /* odd case: n = 2*prefix + 1 */
            seq_t a_even = val + pow2 - (seq_t)1;
            val = a_even + a_even;        /* 2 * a(2*prefix) */
            prefix = (prefix << 1) | 1;
        }
    }

    return val;
}
