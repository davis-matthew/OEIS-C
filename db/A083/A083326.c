/*
 * A083326  a(n) = 6^n - 5^n + 4^n.
 * https://oeis.org/A083326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6^n - 5^n + 4^n
 *
 * The sequence is computed by fast exponentiation for each term.
 * This implementation works for any `seq_t` type supplied by the build
 * (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
 * or an arbitrary‑precision integer class).  The helper
 * `pow_seq` uses binary exponentiation, giving O(log n) time per call.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p6 = pow_seq((seq_t)6, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    return p6 - p5 + p4;
}
