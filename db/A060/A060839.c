/*
 * A060839  Number of solutions to x^3 == 1 (mod n).
 * https://oeis.org/A060839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = 3^(b(n) + (v3(n) >= 2 ? 1 : 0)) where b(n) = number of
 *            distinct prime divisors of n with p ≡ 1 (mod 3), and v3(n) is
 *            the exponent of 3 in n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Predefined primes up to 100 (sqrt(10000)). */
    static const uint32_t primes[25] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
        31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
        73u, 79u, 83u, 89u, 97u
    };
    idx_t temp    = n;
    int   exp3    = 0;
    int   count1  = 0;

    for (int i = 0; i < 25; ++i) {
        uint32_t p = primes[i];
        idx_t pp = (idx_t)p * (idx_t)p;
        if (pp > temp)
            break;
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p == 3u) {
                exp3 += e;
            } else if (p % 3 == 1) {
                ++count1;         /* distinct prime divisor */
            }
        }
    }

    if (temp > 1) {                  /* remaining prime factor > sqrt(n) */
        if (temp == 3u) {
            ++exp3;
        } else if (temp % 3 == 1) {
            ++count1;
        }
    }

    int exponent = count1 + (exp3 >= 2 ? 1 : 0);

    seq_t result = 1;
    for (int i = 0; i < exponent; ++i)
        result *= 3;

    return result;
}
