/*
 * A155078  The representation of n=1,2,3,... in binary is a divisor of the
 *          same representation in another base. The sequence is the first
 *          such base.
 * https://oeis.org/A155078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A155078:  a(n) is the smallest integer x ≥ 3 such that
   the binary digits of n, viewed as a polynomial in x, are ≡ 0 (mod n). */

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    /* Work with an unsigned 64‑bit integer for efficiency. */
    uint64_t nn = (uint64_t)n;

    /* Determine the position of the most significant set bit. */
    int msb = -1;
    for (uint64_t tmp = nn; tmp != 0; tmp >>= 1) {
        ++msb;
    }

    /* Extract the binary digits of n into an array.
       bits[i] holds the coefficient of x^i.  */
    uint8_t bits[64];
    for (int i = 0; i <= msb; ++i) {
        bits[i] = (nn >> i) & 1U;
    }

    /* Search for the least base x ≥ 3 that satisfies the condition. */
    for (uint64_t x = 3; x <= nn + 2; ++x) {
        uint64_t cur = 0;
        for (int i = msb; i >= 0; --i) {
            cur = (cur * x + bits[i]) % nn;
        }
        if (cur == 0) {
            return (seq_t)x;
        }
    }

    /* Theoretically unreachable because a(n) ≤ n+2, but keep
       a fallback to satisfy the compiler. */
    return (seq_t)(nn + 2);
}
