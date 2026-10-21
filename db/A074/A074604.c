/*
 * A074604  a(n) = 2^n + 9^n.
 * https://oeis.org/A074604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A074604.  Uses binary
 * exponentiation to keep the number of multiplications O(log n). */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;        /* current value of 2^k */
    seq_t pow9 = 1;        /* current value of 9^k */
    seq_t base2 = 2;       /* 2^(2^i) during the loop */
    seq_t base9 = 9;       /* 9^(2^i) during the loop */

    idx_t exp = n;
    while (exp) {
        if (exp & 1u) {
            pow2 *= base2;
            pow9 *= base9;
        }
        base2 *= base2;
        base9 *= base9;
        exp >>= 1;
    }
    return pow2 + pow9;
}
