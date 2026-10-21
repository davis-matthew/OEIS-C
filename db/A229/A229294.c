/*
 * A229294  Number of solutions to x^2 + y^2 + z^2 + t^2 == n (mod 2*n) for
 *          x,y,z,t in [0, 2*n).
 * https://oeis.org/A229294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*A240547(n) for odd n, a(2^k) = 24*4^k. For general n =
 *            2^k * m (m odd): a(n) = 8*A240547(m) for k=0, and a(n) =
 *            24*4^k*A240547(m) for k>0. A240547(p^e) = ((p+1)*p^e - 1) *
 *            p^(2e-1) for odd primes p
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 629, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t k = 0;                // exponent of 2
    while ((n & 1ULL) == 0) {
        ++k;
        n >>= 1;
    }
    idx_t m = n;                // odd part

    seq_t a_odd = 1;            // product of A240547(p^e) over odd prime factors

    if (m > 1) {
        idx_t temp = m;
        for (idx_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }

                seq_t pe = pow_seq((seq_t)p, e);          // p^e
                seq_t p_2e_minus1 = pow_seq((seq_t)p, 2*e - 1); // p^(2e-1)
                seq_t term = ((seq_t)(p + 1) * pe) - 1;   // (p+1)*p^e - 1
                term *= p_2e_minus1;                     // * p^(2e-1)

                a_odd *= term;
            }
        }
        if (temp > 1) {      // remaining prime factor
            idx_t p = temp;
            idx_t e = 1;
            seq_t pe = (seq_t)p;
            seq_t p_2e_minus1 = (seq_t)p;           // p^(2*1-1) = p
            seq_t term = ((seq_t)(p + 1) * pe) - 1; // (p+1)*p - 1
            term *= p_2e_minus1;                  // * p
            a_odd *= term;
        }
    }

    if (k == 0) {            // n is odd
        return 8 * a_odd;
    } else {
        seq_t pow4k = pow_seq(4, k);             // 4^k
        return 24 * pow4k * a_odd;              // 24*4^k*A240547(m)
    }
}
