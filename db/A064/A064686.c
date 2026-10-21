/*
 * A064686  a(n) = number of n-digit base-3 biquams.
 * https://oeis.org/A064686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 3^(n-1) - 2^(n-2) for n ≥ 2, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence a(n) = 5*a(n-1) - 6*a(n-2) for n > 3 is also valid,
   but the closed form is simpler to evaluate. */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    seq_t pow3 = pow_seq_t((seq_t)3, n - 1);
    seq_t pow2 = pow_seq_t((seq_t)2, n - 2);
    return pow3 - pow2;
}
