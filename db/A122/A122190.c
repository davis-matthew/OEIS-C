/*
 * A122190  Expansion of q^(-1/4) * eta(q^2) * eta(q^5)^3 / (eta(q) *
 *          eta(q^10)) in powers of q.
 * https://oeis.org/A122190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(4*n + 1) where b is multiplicative: • b(2^e) = 0^e
 *            (zero if e>0); • b(5^e) = 1; • for primes p ≡ 1 (mod 4): b(p^e)
 *            = e+1; • for primes p ≡ 3 (mod 4): b(p^e) = (1 + (-1)^e)/2. Thus
 *            a(n) = 0 if 4*n+1 contains a prime ≡ 3 (mod 4) to an odd power,
 *            otherwise a(n) = ∏_{p≡1 mod 4} (e+1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4*n + 1 is always odd, so factorization only needs odd divisors. */
    idx_t m = 4*n + 1;           /* value to factorize */
    seq_t  res = 1;              /* multiplicative accumulator */
    idx_t  rem = m;              /* remaining part to factor */

    /* trial division by odd numbers up to sqrt(rem) */
    for (idx_t d = 3; d*d <= rem; d += 2) {
        if (rem % d == 0) {
            /* count exponent of prime d */
            idx_t e = 0;
            while (rem % d == 0) {
                rem /= d;
                e++;
            }

            if (d == 5) {
                /* factor 5 contributes nothing to the product */
                continue;
            } else if ((d & 3) == 1) {        /* d ≡ 1 (mod 4) */
                res *= (seq_t)(e + 1);
            } else {                         /* d ≡ 3 (mod 4) */
                if (e & 1) return (seq_t)0;
            }
        }
    }

    /* If remainder > 1, it is a prime factor > sqrt(m). */
    if (rem > 1) {
        if (rem == 5) {
            /* ignore */
        } else if ((rem & 3) == 1) {    /* rem ≡ 1 (mod 4) */
            res *= (seq_t)2;            /* exponent e = 1 => e+1 = 2 */
        } else {                        /* rem ≡ 3 (mod 4) */
            return (seq_t)0;
        }
    }

    return res;
}
