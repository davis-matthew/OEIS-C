/*
 * A162699  Odd numbers not divisible by 7.
 * https://oeis.org/A162699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 1 + 2*floor((2*n-7)/12) (OEIS A162699)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the few cases where the floor term is negative */
    if (n <= 3) {
        static const seq_t vals[3] = {1, 3, 5};
        return vals[n - 1];
    }

    /* 2*n is safe for seq_t because for n >= 4 the numerator is positive */
    seq_t two_n   = (seq_t)2 * (seq_t)n;
    seq_t num     = two_n - (seq_t)7;          /* 2*n - 7  (non‑negative for n>=4) */
    seq_t floorv  = num / (seq_t)12;           /* floor((2*n-7)/12) */
    seq_t result  = two_n + (seq_t)1 + (seq_t)2 * floorv;
    return result;
}
