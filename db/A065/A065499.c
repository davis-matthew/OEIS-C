/*
 * A065499  Noninvertible 3 X 3 matrices over Z_n.
 * https://oeis.org/A065499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^9 - n^9 * Π (1 - 1/p^3)(1 - 1/p^2)(1 - 1/p), where the
 *            product is over the distinct prime divisors p of n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 143, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer power for seq_t – works for all built‑in and arbitrary‑precision builds */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* trial factorisation of n (uint64_t). Returns prime/exponent pairs in
   the supplied arrays, zeroing the remaining entries.  The caller supplies
   arrays large enough for the number of distinct prime factors (≤ 32). */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t i = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            pr[i]   = d;
            exp[i]  = e;
            ++i;
        }
        d += (d == 2) ? 1 : 2;   /* 2,3,5,7,... */
    }
    if (n > 1) {                 /* remaining prime factor */
        pr[i]   = n;
        exp[i]  = 1;
        ++i;
    }
    *cnt = i;
}

/* a(idx_t n) – number of non‑invertible 3×3 matrices over Z_n */
seq_t a(idx_t n)
{
    /* n^9 */
    seq_t n9 = pow_seq_t((seq_t)n, 9);

    /* factorise n */
    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_uint64((uint64_t)n, pr, ex, &cnt);

    /* invertible count = n9 * Π (p^3-1)(p^2-1)(p-1) / p^6
       = Π p^(9e-6) * (p^3-1)(p^2-1)(p-1)   */
    seq_t inv = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)pr[i];
        uint64_t e = ex[i];
        idx_t pow_exp = 9 * (idx_t)e - 6;          /* 9e-6  */
        seq_t term   = pow_seq_t(p, pow_exp);
        term         = term * (pow_seq_t(p, 3) - 1);
        term         = term * (pow_seq_t(p, 2) - 1);
        term         = term * (p - 1);
        inv = inv * term;
    }

    /* a(n) = n^9 - invertible count */
    return n9 - inv;
}
