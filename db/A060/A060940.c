/*
 * A060940  Triangle in which n-th row gives the phi(n) terms appearing as
 *          initial primes in arithmetic progressions with difference n, with
 *          initial term equal to the smallest positive residue coprimes to n.
 * https://oeis.org/A060940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 8950, u32 n <= 8950, u64 n <= 8950,
 *            u128 n <= 8950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060940: a(n) is the first prime in the arithmetic progression r + k*n (k≥0)
   where r is the (n‑cum_φ)-th integer 1≤r<n with gcd(r,n)=1, cum_φ being the sum
   of φ(i) for i< n.  This implementation searches for that prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Euclidean gcd */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Euler's totient φ for 64‑bit integers */
static inline idx_t phi_u64(idx_t n) {
    idx_t res = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / p;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

/* modular exponentiation (uint64_t, base^exponent mod mod) */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
        17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (int r = 1; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, return 0 */

    /* find row r so that sum_{i<r} φ(i) < n ≤ sum_{i≤r} φ(i) */
    idx_t cum = 0;
    idx_t r = 1;
    while (1) {
        idx_t ph = phi_u64(r);
        if (cum + ph >= n) break;
        cum += ph;
        ++r;
    }

    idx_t offset = n - cum - 1;        /* 0‑based index in the row */

    /* determine the (offset)‑th residue coprime to r */
    uint64_t residue;
    if (r == 1) {
        residue = 1;
    } else {
        idx_t count = 0;
        uint64_t res = 1;
        for (idx_t x = 1; x < r; ++x) {
            if (gcd_u64(x, r) == 1) {
                if (count == offset) { res = x; break; }
                ++count;
            }
        }
        residue = res;
    }

    /* search for the first prime > residue in the AP residue + k*r */
    uint64_t step = (uint64_t)r;
    uint64_t cand = residue + step;
    while (!is_prime_uint64(cand)) cand += step;

    return (seq_t)cand;
}
