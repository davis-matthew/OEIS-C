/*
 * A182665  Greatest x < n such that n divides x*(x-1).
 * https://oeis.org/A182665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest x < n such that n divides x*(x-1) (OEIS A182665)
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* extended gcd for int64_t */
static int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* Chinese remainder combine: find r such that
   r ≡ a1 (mod m1)
   r ≡ a2 (mod m2)
   (m1 and m2 are coprime). Returns residue modulo m1*m2. */
static uint64_t crt_combine(uint64_t a1, uint64_t m1, uint64_t a2, uint64_t m2)
{
    int64_t x, y;
    egcd((int64_t)m1, (int64_t)m2, &x, &y);
    int64_t inv = x % (int64_t)m2;
    if (inv < 0) inv += m2;

    int64_t diff = (int64_t)a2 - (int64_t)a1;
    diff %= (int64_t)m2;
    if (diff < 0) diff += m2;

    __int128 t = (__int128)diff * inv % (int64_t)m2;
    __int128 res = (__int128)a1 + (__int128)m1 * t;
    uint64_t product = (uint64_t)((__int128)m1 * m2);
    return (uint64_t)(res % product);
}

/* factorize n into prime powers.  pf holds primes, pe holds prime powers.
   len is set to the number of distinct prime powers. */
static void factorize(uint64_t n, uint64_t pf[64], uint64_t pe[64], size_t *len)
{
    *len = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
            }
            pf[*len] = p;
            pe[*len] = power;
            (*len)++;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        pf[*len] = temp;
        pe[*len] = temp;
        (*len)++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t primes[64], powers[64];
    size_t k;
    factorize((uint64_t)n, primes, powers, &k);

    uint64_t best = 0;
    uint64_t masks = ((uint64_t)1 << k);
    for (uint64_t mask = 0; mask < masks; ++mask) {
        uint64_t r = 0;
        uint64_t m = 1;
        for (size_t i = 0; i < k; ++i) {
            uint64_t a2 = (mask & ((uint64_t)1 << i)) ? 1 : 0;   /* residue 1 or 0 mod prime power */
            r = crt_combine(r, m, a2, powers[i]);
            m = (uint64_t)((__int128)m * powers[i]);            /* cumulative modulus */
        }
        if (r > best) best = r;
    }
    return (seq_t)best;
}
