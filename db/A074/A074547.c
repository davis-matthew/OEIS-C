/*
 * A074547  a(n) = 3^n + 4^n + 5^n.
 * https://oeis.org/A074547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 4^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp for arbitrary-precision integer types seq_t
   using binary exponentiation.  This helper uses only the
   builtin operators (+, *, &, >>) that are defined for seq_t. */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1;
        if (exp != 0)
            base *= base;
    }
    return result;
}

/* Main function required by the test harness.  It returns
   the nth term (a(0) = 3) of the sequence A074547. */
seq_t a(idx_t n)
{
    seq_t p3 = seq_pow((seq_t)3, n);
    seq_t p4 = seq_pow((seq_t)4, n);
    seq_t p5 = seq_pow((seq_t)5, n);
    return p3 + p4 + p5;
}
