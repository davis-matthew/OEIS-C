/*
 * A101942  Write n in base 4 as n = b_0 + b_1*4 + b_2*4^2 + b_3*4^3 + ...;
 *          then a(n) = Product_{i >= 0} prime(i+1)^b_i.
 * https://oeis.org/A101942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = ∏_{i≥0} prime(i+1)^{b_i}, * where n = Σ_{i≥0} b_i·4^i (0
 *            ≤ b_i ≤ 3)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 32 primes, sufficient for any 64‑bit n (base‑4 has ≤32 digits) */
    static const seq_t primes[32] = {
        (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)11, (seq_t)13,
        (seq_t)17, (seq_t)19, (seq_t)23, (seq_t)29, (seq_t)31, (seq_t)37,
        (seq_t)41, (seq_t)43, (seq_t)47, (seq_t)53, (seq_t)59, (seq_t)61,
        (seq_t)67, (seq_t)71, (seq_t)73, (seq_t)79, (seq_t)83, (seq_t)89,
        (seq_t)97, (seq_t)101, (seq_t)103, (seq_t)107, (seq_t)109, (seq_t)113,
        (seq_t)127, (seq_t)131
    };

    seq_t   result = 1;
    idx_t   x      = n;
    idx_t   i      = 0;

    while (x != 0) {
        idx_t digit = x & 3ULL;      /* x % 4 */
        if (digit) {
            seq_t p = primes[i];
            switch ((int)digit) {
                case 1: result *= p;                 break;
                case 2: result *= p * p;             break;
                case 3: result *= p * p * p;         break;
            }
        }
        x >>= 2;   /* x /= 4 */
        ++i;
    }
    return result;
}
