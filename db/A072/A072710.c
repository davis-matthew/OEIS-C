/*
 * A072710  Last digit of F(n) is 8 where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A072710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 60*((n-1)/4) + (6,24,27,33)[(n-1)%4]
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence of Fibonacci indices ≤ 60 with last digit 8 */
    static const seq_t arr[4] = { (seq_t)6, (seq_t)24,
                                  (seq_t)27, (seq_t)33 };
    idx_t k   = (n - 1) / 4;          /* how many full 60‑periods */
    idx_t r   = (n - 1) % 4;          /* position inside the period */
    seq_t res = (seq_t)k * (seq_t)60 + arr[r];
    return res;
}
