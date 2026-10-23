/*
 * A167356  Totally multiplicative sequence with a(p) = (p-2)*(p-3) = p^2-5p+6
 *          for prime p.
 * https://oeis.org/A167356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p-2)*(p-3))^e, a(1)=1; a(n)=0 if 2∣n or 3∣n
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined case */

    if ((n & 1) == 0 || (n % 3) == 0) /* divisible by 2 or 3 */
        return 0;

    idx_t m = n;
    seq_t result = 1;

    for (idx_t p = 5; p * p <= m; p += 6)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            seq_t val = (seq_t)(p * p) - (seq_t)5 * (seq_t)p + 6;
            seq_t val_pow = 1;
            idx_t exp = e;
            while (exp)
            {
                if (exp & 1)
                    val_pow *= val;
                val *= val;
                exp >>= 1;
            }
            result *= val_pow;
        }

        idx_t p2 = p + 2;
        if (p2 * p2 > m)
            break;
        if (m % p2 == 0)
        {
            idx_t e = 0;
            while (m % p2 == 0)
            {
                m /= p2;
                ++e;
            }
            seq_t val = (seq_t)(p2 * p2) - (seq_t)5 * (seq_t)p2 + 6;
            seq_t val_pow = 1;
            idx_t exp = e;
            while (exp)
            {
                if (exp & 1)
                    val_pow *= val;
                val *= val;
                exp >>= 1;
            }
            result *= val_pow;
        }
    }

    if (m > 1) /* remaining prime factor */
    {
        seq_t val = (seq_t)(m * m) - (seq_t)5 * (seq_t)m + 6;
        result *= val;
    }

    return result;
}
