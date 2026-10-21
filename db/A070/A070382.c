/*
 * A070382  a(n) = 5^n mod 34.
 * https://oeis.org/A070382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n mod 34
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 34;
    seq_t base = 5 % MOD;
    /* Euler's totient φ(34) = 16, 5 and 34 are coprime, so 5^n ≡ 5^(n mod 16) (mod 34) */
    seq_t exp = n % 16;
    seq_t result = 1;

    while (exp) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    return result;
}
