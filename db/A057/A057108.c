/*
 * A057108  Difference between the smallest number S(n) with S(n)! a multiple
 *          of n and the largest prime factor of n [taking a(1)=0].
 * https://oeis.org/A057108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9959 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = S(n) - LPF(n)
 *
 *  S(n)  = Smarandache function (smallest k such that n divides k!)
 *  LPF(n)= largest prime factor of n
 *
 * The function works for all n ≥ 1 and uses only integer arithmetic.
 * No hard‑coded table of values is used; prime factorizations are obtained
 * with the helper routine factor().
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Special case from the OEIS entry */
    if (n == (idx_t)1)
        return (seq_t)0;

    /* Factorise n */
    uint64_t pr[64], exp[64];
    size_t cnt;
    factor(n, pr, exp, &cnt);

    idx_t max_k = 0;        /* S(n) */
    idx_t largest_prime = 0;/* LPF(n) */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = pr[i];
        idx_t e = exp[i];

        /* Smallest k such that exponent of p in k! ≥ e */
        idx_t k = 1;
        while (1) {
            idx_t tmp = k;
            idx_t exp_acc = 0;
            while (tmp) {
                tmp /= p;
                exp_acc += tmp;
            }
            if (exp_acc >= e)
                break;
            ++k;
        }

        if (k > max_k)
            max_k = k;
        if (p > largest_prime)
            largest_prime = p;
    }

    /* a(n) = S(n) – LPF(n) */
    return (seq_t)(max_k - largest_prime);
}
