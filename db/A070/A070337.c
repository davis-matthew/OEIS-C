/*
 * A070337  a(n) = 2^n mod 27.
 * https://oeis.org/A070337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 27
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 18;               /* 2^18 ≡ 1 (mod 27) */
    idx_t e = n % period;                  /* reduce exponent modulo period */

    seq_t result = 1;
    seq_t base   = 2;                      /* 2 mod 27 */

    while (e) {
        if (e & 1) {
            result = (seq_t)((unsigned long long)result *
                              (unsigned long long)base % 27);
        }
        base = (seq_t)((unsigned long long)base *
                       (unsigned long long)base % 27);
        e >>= 1;
    }
    return result;
}
