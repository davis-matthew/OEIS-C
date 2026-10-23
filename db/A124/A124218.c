/*
 * A124218  a(n) is the m-th positive integer which is coprime to n, where
 *          phi(n) is number of positive integers which are <= n and are
 *          coprime to n, d(n) is the number of positive divisors of n and m =
 *          phi(n) if phi(n)|d(n), else m = d(n) mod phi(n).
 * https://oeis.org/A124218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 8885, u32 n <= 8885, u64 n <= 8885,
 *            u128 n <= 8885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = m-th positive integer that is coprime to n,
 * where
 *   m = φ(n)            if φ(n) divides d(n)
 *   m = d(n) mod φ(n)   otherwise
 * φ(n) is Euler's totient, d(n) is the number of positive divisors of n.
 */
 
#include <stdint.h>
 
/* Euclidean algorithm for seq_t */
static inline seq_t __gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}
 
/* Euler's totient function for small n (≤ 10000 or less) */
static inline seq_t __phi(idx_t n)
{
    seq_t result = n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            result = (result / p) * (p - 1);
        }
    }
    if (temp > 1) {
        result = (result / temp) * (temp - 1);
    }
    return result;
}
 
/* Number of positive divisors of n (n ≤ 10000, fits in uint64_t) */
static inline uint64_t __divisor_count_u64(uint64_t n)
{
    uint64_t d = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            d += (i * i == n) ? 1 : 2;
        }
    }
    return d;
}
 
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n = 0, return 0 as a fallback */
        return (seq_t)0;
 
    seq_t phi_n = __phi(n);
    uint64_t d_n = __divisor_count_u64(n);
    seq_t m;
    if (d_n % phi_n == 0)
        m = phi_n;
    else
        m = (seq_t)(d_n % phi_n);
 
    /* m is guaranteed to be positive */
    idx_t mi = (idx_t)m;   /* safe: m ≤ φ(n) ≤ n ≤ 10000 */
 
    idx_t count = 0;
    idx_t i = 1;
    for (;;) {
        if (__gcd_seq((seq_t)i, (seq_t)n) == 1) {
            ++count;
            if ((idx_t)count == mi) {
                return (seq_t)i;
            }
        }
        ++i;
    }
}
