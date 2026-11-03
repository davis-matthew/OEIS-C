/*
 * A185278  Number of isomorphism classes of generalized Petersen graphs
 *          G(n,k) on 2n vertices with gcd(n,k) = 1.
 * https://oeis.org/A185278
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (phi(n) + A060594(n) + A000089(n)) / 4 * where A060594(n)
 *            = number of solutions to x^2 ≡ 1 (mod n) * and A000089(n) =
 *            number of solutions to x^2 ≡ -1 (mod n)
 * Range:     u8 n <= 897, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9125 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Value type aliases (supplied by the build) */
#include <stdint.h>
#include <stdbool.h>

/* Count solutions to x^2 ≡ 1 (mod n)  (A060594) */
static seq_t _solutions_x2eq1(uint64_t n) {
    uint64_t temp = n;
    int e_pow2 = 0;
    while (temp % 2 == 0) {
        temp /= 2;
        e_pow2++;
    }

    /* factor base contributed by the 2-adic part */
    uint64_t base;
    if (e_pow2 <= 1)          base = 1ULL;   /* e = 0 or 1 */
    else if (e_pow2 == 2)     base = 2ULL;   /* e = 2   */
    else                      base = 4ULL;   /* e ≥ 3   */

    /* count distinct odd prime factors */
    uint64_t odd = temp;
    int t = 0;
    uint64_t odd_tmp = odd;
    for (uint64_t p = 3; p * p <= odd_tmp; p += 2) {
        if (odd_tmp % p == 0) {
            t++;
            while (odd_tmp % p == 0) odd_tmp /= p;
        }
    }
    if (odd_tmp > 1) t++;          /* remaining prime factor */

    return base * ((uint64_t)1 << t);
}

/* Count solutions to x^2 ≡ -1 (mod n)  (A000089) */
static seq_t _solutions_x2eqm1(uint64_t n) {
    /* No solutions when 4 | n */
    if (n % 4 == 0) return (seq_t)0;

    /* strip factor 2 (at most one) */
    uint64_t odd = n;
    while (odd % 2 == 0) odd /= 2;

    /* n == 1 or 2 -> exactly one solution (x ≡ 1) */
    if (odd == 1) return (seq_t)1;

    /* Check every odd prime factor */
    int t = 0;          /* count of distinct primes ≡ 1 (mod 4) */
    bool bad = false;   /* flag if any prime ≡ 3 (mod 4) appears */
    uint64_t odd_tmp = odd;
    for (uint64_t p = 3; p * p <= odd_tmp; p += 2) {
        if (odd_tmp % p == 0) {
            if (p % 4 == 3) { bad = true; break; }
            t++;
            while (odd_tmp % p == 0) odd_tmp /= p;
        }
    }
    if (!bad && odd_tmp > 1) {
        if (odd_tmp % 4 == 3) bad = true;
        else t++;
    }

    if (bad) return (seq_t)0;
    return ((uint64_t)1 << t);
}

/* Small helper: compute phi(n) efficiently */
static seq_t _phi_uint64(uint64_t n) {
    uint64_t result = n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return (seq_t)result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;   /* undefined for n < 3 */

    uint64_t n64 = (uint64_t)n;

    seq_t phi_val = _phi_uint64(n64);
    seq_t B = _solutions_x2eq1(n64);
    seq_t C = _solutions_x2eqm1(n64);

    return (phi_val + B + C) / 4;
}
