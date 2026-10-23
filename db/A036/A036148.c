/*
 * A036148  6^n mod 151.
 * https://oeis.org/A036148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 151
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mulmod(seq_t a, seq_t b, seq_t m)
{
    /* Use 64‑bit arithmetic to avoid overflow for small seq_t types.
       For big‑int seq_t the cast is safe because a,b,m < 151. */
    return (seq_t)(((unsigned long long)a * (unsigned long long)b) % (unsigned long long)m);
}

seq_t a(idx_t n)
{
    const seq_t mod = 151;
    seq_t result = 1;
    seq_t base   = 6;

    while (n > 0) {
        if (n & 1ULL) {
            result = mulmod(result, base, mod);
        }
        base = mulmod(base, base, mod);
        n >>= 1;
    }
    return result;
}
