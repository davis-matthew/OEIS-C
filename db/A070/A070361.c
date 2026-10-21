/*
 * A070361  a(n) = 3^n mod 41.
 * https://oeis.org/A070361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 41
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 41;
    seq_t result = 1;
    seq_t base   = 3;
    idx_t e = n;

    while (e > 0) {
        if (e & 1) {
            result = (seq_t)((unsigned long long)result * (unsigned long long)base % MOD);
        }
        e >>= 1;
        if (e == 0) break;
        base = (seq_t)((unsigned long long)base * (unsigned long long)base % MOD);
    }

    return result;
}
