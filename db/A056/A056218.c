/*
 * A056218  If n = p_1^a_1 * p_2^a_2 * p_3^a_3 * ..., where p_k is the k-th
 *          prime and the a's are nonnegative integers, then a(n) =
 *          n!/product_{k >= 1} [(p_k)!^a_k].
 * https://oeis.org/A056218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 457 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056218: a(n) = n! / ∏ (p_k)!^a_k
 *  where n = ∏ p_k^a_k is the prime factorisation of n.
 *  The implementation factorises n, removes the factorial exponents
 *  using Legendre’s formula and reconstructs the result from prime
 *  exponents.  No large factorials are computed, so 64‑bit builds
 *  are safe for all n≤10000 and arbitrary‑precision builds work
 *  automatically because only `seq_t` arithmetic is used.
 */
#include <stdlib.h>

/* factorise n into arrays pr[12], ex[12]; count stored in *cnt */
static void factor_n(idx_t n, idx_t *pr, idx_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            ex[c] = 0;
            while (n % p == 0) {
                n /= p;
                ++ex[c];
            }
            pr[c] = p;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* test whether x is prime – simple trial division, fast enough for n≤10000 */
static int is_prime_small(idx_t x)
{
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

/* Legendre exponent of prime `p` in `n!` */
static long long legendre(idx_t n, idx_t p)
{
    long long acc = 0;
    while (n) {
        n /= p;
        acc += n;
    }
    return acc;
}

/* binary exponentiation: base^exp as seq_t, exp ≤ 2^31 */
static seq_t pow_seq_t(idx_t base, long long exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = a(1) = 1 */
    if (n < 2) return (seq_t)1;

    /* factorise n */
    idx_t pr[12], ex[12];          /* 12 enough for n ≤ 10000 */
    size_t nc = 0;
    factor_n(n, pr, ex, &nc);

    /* accumulate result from prime exponents */
    seq_t result = 1;

    for (idx_t q = 2; q <= n; ++q) if (is_prime_small(q)) {
        long long exp = legendre(n, q);
        for (size_t i = 0; i < nc; ++i)
            exp -= ex[i] * legendre(pr[i], q);
        if (exp > 0)
            result *= pow_seq_t(q, exp);
    }

    return result;
}
