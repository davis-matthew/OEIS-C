/*
 * A091243  Differences between consecutive reducible GF(2)[X]-polynomials.
 * https://oeis.org/A091243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A091243
 *  Differences between consecutive reducible GF(2)[X]-polynomials.
 *  a(n) = value_of_r(n+1) – value_of_r(n)
 *  where r(n) is the n-th reducible monic polynomial over GF(2),
 *  ordered by the binary value of its coefficient vector (constant term LSB).
 *
 *  The implementation tests each integer ≥4 for reducibility over GF(2)
 *  using a standard irreducibility test based on modular arithmetic
 *  in GF(2)[X].
 */

#include <stdint.h>
#include <stdbool.h>

static inline int deg_uint64(uint64_t x) {
    if (x == 0) return -1;
    return 63 - __builtin_clzll(x);
}

/* Polynomial remainder: a mod b over GF(2) */
static inline uint64_t poly_mod_r(uint64_t a, uint64_t b) {
    int db = deg_uint64(b);
    if (db < 0) return a;
    int da = deg_uint64(a);
    while (da >= db && a) {
        a ^= b << (da - db);
        da = deg_uint64(a);
    }
    return a;
}

/* Polynomial gcd over GF(2) */
static inline uint64_t poly_gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t r = poly_mod_r(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Modular multiplication of polynomials over GF(2) */
static inline uint64_t poly_mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 0;
    uint64_t deg_mod = (uint64_t)deg_uint64(mod);
    while (b) {
        if (b & 1ULL) res ^= a;
        b >>= 1;
        /* multiply a by x */
        a <<= 1;
        /* reduce if degree exceeds mod */
        if (deg_uint64(a) >= deg_mod) {
            a ^= mod << (deg_uint64(a) - deg_mod);
        }
    }
    return res;
}

/* Test whether a monic polynomial (as 64‑bit integer) is irreducible over GF(2) */
static inline bool is_irreducible(uint64_t p) {
    int n = deg_uint64(p);
    if (n <= 1) return false;          /* degree 0 or 1 is not considered reducible here */
    uint64_t x = 2ULL;                 /* polynomial x */
    uint64_t t = x;                    /* will hold x^(2^k) mod p */
    for (int k = 1; k <= n / 2; ++k) {
        t = poly_mod_mul(t, t, p);              /* square */
        uint64_t g = poly_gcd(p, t ^ x);        /* check gcd with x */
        if (g != 1ULL) return false;           /* reducible */
    }
    return true;                            /* irreducible */
}

/* Test whether a monic polynomial is reducible over GF(2) */
static inline bool is_reducible(uint64_t p) {
    if (p < 4ULL) return false;      /* degrees 0 or 1 cannot be reducible */
    return !is_irreducible(p);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but never used */
    idx_t count = 0;
    seq_t prev = 0;
    for (seq_t cur = 4; ; ++cur) {         /* start from polynomial x^2 (value 4) */
        if (is_reducible((uint64_t)cur)) {
            if (prev) {                     /* not the first reducible polynomial */
                ++count;
                if (count == n) return cur - prev;
            }
            prev = cur;
        }
    }
}
