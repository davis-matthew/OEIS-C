/*
 * A236852  Remultiply n first "downward", from N to GF(2)[X], and then
 *          remultiply that result back "upward", from GF(2)[X] to N: a(n) =
 *          A234742(A234741(n)).
 * https://oeis.org/A236852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A234742(A234741(n))
 * Range:     u8 n <= 67, u16 n <= 2569, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* return the number of bits needed to represent x (0 → 0) */
static inline int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Polynomial division over GF(2):
   a = b * q + r   (mod 2)            */
static inline void poly_divmod(seq_t a, seq_t b, seq_t *q, seq_t *r)
{
    seq_t aa = a;
    seq_t qq = 0;

    if (b == 0) {                     /* not used, but harmless */
        if (q) *q = 0;
        if (r) *r = aa;
        return;
    }

    int da = bitlen_seq_t(aa) - 1;
    int db = bitlen_seq_t(b)  - 1;

    while (da >= db && aa != 0) {
        int shift = da - db;
        qq ^= ((seq_t)1 << shift);
        aa ^= (b << shift);

        da = bitlen_seq_t(aa) - 1;
    }

    if (q) *q = qq;
    if (r) *r = aa;
}

/* recursively factor a polynomial over GF(2) and return
   the product of the integer representations of all irreducible
   factors (with multiplicity). */
static seq_t factor_product(seq_t f)
{
    if (f <= 1)       /* 0 or 1 -> product is f itself */
        return f;

    int deg = bitlen_seq_t(f) - 1;
    for (int d = 1; d <= deg / 2; ++d) {          /* try possible divisor degrees */
        int ways = 1 << d;                        /* number of masks of lower bits */
        seq_t high = ((seq_t)1 << d);             /* monic candidate must have bit d set */

        for (int mask = 0; mask < ways; ++mask) {
            seq_t cand = high | mask;             /* monic polynomial of degree d */

            seq_t q, r;
            poly_divmod(f, cand, &q, &r);
            if (r == 0) {                         /* cand divides f */
                seq_t left  = factor_product(cand);
                seq_t right = factor_product(q);
                return left * right;
            }
        }
    }

    /* no divisor found → f is irreducible */
    return f;
}

/* compute a(p) for a prime p:
   the product of the integer representations of the irreducible
   factors of the polynomial encoded by p. */
static seq_t a_prime(idx_t p)
{
    return factor_product((seq_t)p);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t temp   = n;

    /* trial division – n is small (≤ 10 000 in tests), so this is fast enough */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t ap = a_prime(p);
            seq_t pow = 1;
            while (e--) pow *= ap;
            result *= pow;
        }
    }

    if (temp > 1) {                     /* remaining prime factor */
        seq_t ap = a_prime(temp);
        result *= ap;
    }

    return result;
}
