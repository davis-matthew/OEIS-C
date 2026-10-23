/*
 * A173807  a(n) = (7*10^n - 43)/9 for n > 0.
 * https://oeis.org/A173807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+43 with a(1)=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A173807.
   Uses the recurrence from the OEIS entry to avoid overflow
   of the intermediate 7*10^n form. Works for all supported
   seq_t types (unsigned integers or arbitrary‑precision). */
seq_t a(idx_t n)
{
    if (n == 1)
        return 3;          /* a(1) = 3 */
    seq_t val = 3;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = val * 10 + 43;   /* a(i) = 10*a(i-1)+43 */
    return val;
}
