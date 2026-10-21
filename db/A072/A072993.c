/*
 * A072993  Numbers n such that phi(n) divides C(2n,n).
 * https://oeis.org/A072993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072993: numbers m such that φ(m) divides C(2m,m). */
/* The function returns the n‑th smallest such m (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small list of primes up to 199 – sufficient to factor all numbers <= 30000 */
static const uint32_t primes_list[] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199
};
static const size_t primes_cnt = sizeof(primes_list) / sizeof(primes_list[0]);

/* Factorisation of n (n <= 30000) into primes */
static void factor_u64_local(idx_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    size_t i = 0;
    while (i < primes_cnt && (uint64_t)primes_list[i] * primes_list[i] <= n) {
        uint32_t p = primes_list[i];
        if (n % p == 0) {
            size_t e = 0;
            do {
                n /= p;
                ++e;
            } while (n % p == 0);
            pr[c]   = p;
            exp[c]  = e;
            ++c;
        }
        ++i;
    }
    if (n > 1) {
        pr[c]  = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Euler's totient function using the factorisation above */
static idx_t phi_ul(idx_t n)
{
    if (n == 0) return 0;
    idx_t result = n;
    uint64_t pr[32], exp[32];
    size_t cnt, i;
    factor_u64_local(n, pr, exp, &cnt);
    for (i = 0; i < cnt; ++i) {
        result = result / pr[i] * (pr[i] - 1);
    }
    return result;
}

/* Exponent of prime p in n! (Legendre's formula) */
static size_t legendre_exp(idx_t n, uint32_t p)
{
    size_t exp = 0;
    idx_t div = n;
    while (div) {
        div /= p;
        exp += div;
    }
    return exp;
}

/* Test whether a given integer m satisfies φ(m) | C(2m,m) */
static bool good_candidate(idx_t m)
{
    if (m == 0) return false;
    idx_t phi = phi_ul(m);

    uint64_t pr2[32], exp2[32];
    size_t cnt2, i;
    factor_u64_local(phi, pr2, exp2, &cnt2);

    for (i = 0; i < cnt2; ++i) {
        uint32_t q = (uint32_t)pr2[i];
        size_t exp_phi = exp2[i];
        size_t exp_bin = legendre_exp(2 * m, q) - 2 * legendre_exp(m, q);
        if (exp_bin < exp_phi) return false;
    }
    return true;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    static idx_t *seq = NULL;          /* sequence storage */
    static size_t seq_len = 0;         /* number of terms found */
    static size_t seq_cap = 0;         /* allocated capacity */
    static idx_t candidate = 0;        /* next integer to test */

    while (seq_len < n) {
        ++candidate;
        if (good_candidate(candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 32;
                seq = (idx_t *)realloc(seq, seq_cap * sizeof(idx_t));
            }
            seq[seq_len++] = candidate;
        }
    }
    return (seq_t)seq[n - 1];
}
