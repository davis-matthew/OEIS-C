/*
 * A063376  a(-1) = 1; for n >= 0, a(n) = 2^n + 4^n = 2^n*(1 + 2^n).
 * https://oeis.org/A063376
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 4^n (a(-1) = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    /* fast exponentiation by squaring, works for both fixed‑size
       integer types and arbitrary‑precision integers. */
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == -1) return (seq_t)1;      /* special offset term */
    /* n >= 0 : a(n) = 2^n + 4^n */
    return pow_seq((seq_t)2, n) + pow_seq((seq_t)4, n);
}
