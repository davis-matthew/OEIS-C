/*
 * A016006  a(n) = (tau(n^10)+9)/10.
 * https://oeis.org/A016006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (τ(n^10) + 9) / 10
 * Range:     u8 n <= 23, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(k) is the number of positive divisors of k.
   For n = Π p_i ^ e_i, τ(n^10) = Π (10*e_i + 1) */
seq_t a(idx_t n)
{
    uint64_t nn = n;
    seq_t prod = 1;           /* product of (10*e_i + 1) */
    uint64_t temp = nn;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prod *= (seq_t)(10 * e + 1);
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        prod *= (seq_t)(10 * 1 + 1);
    }

    prod += 9;
    return prod / 10;
}
