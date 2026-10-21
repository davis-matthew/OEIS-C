/*
 * A079612  Largest number m such that a^n == 1 (mod m) whenever a is coprime
 *          to m.
 * https://oeis.org/A079612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is odd a(n) = 2^(t+2) * ∏ p^(v_p(n)+1) where t =
 *            v2(n) (exponent of 2 in n) and the product runs over all odd
 *            primes p such that (p-1) divides n. (v_p(n) is the exponent of
 *            prime p in n.)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 59, u64 n <= 419,
 *            u128 n <= 2519, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- small helper: exponentiation with seq_t base ---- */
static seq_t pow_seq_uint(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp--) {
        res *= base;
    }
    return res;
}

/* --- naive primality test for uint64_t --- */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/*----------------------------------------------------*/

seq_t a(idx_t n)
{
    /* Odd indices */
    if (n & 1ULL)
        return 2;

    /* t = exponent of 2 in n */
    idx_t tmp = n;
    idx_t t = 0;
    while ((tmp & 1ULL) == 0) {
        t++;
        tmp >>= 1;
    }

    /* Result starts with 2^(t+2) */
    seq_t result = pow_seq_uint(2, t + 2);

    /* For each odd prime p such that (p-1) | n,
       multiply by p^(v_p(n)+1) */
    idx_t limit = n + 1;                       /* upper bound for p    */
    for (idx_t p = 3; p <= limit; p += 2) {
        if (n % (p - 1) != 0) continue;        /* (p-1) must divide n */
        if (!is_prime_uint64(p)) continue;      /* p must be prime    */

        /* compute v_p(n) */
        idx_t vp = 0;
        idx_t temp = n;
        while (temp % p == 0) {
            vp++;
            temp /= p;
        }

        seq_t pow_val = pow_seq_uint((seq_t)p, vp + 1);
        result *= pow_val;
    }

    return result;
}
