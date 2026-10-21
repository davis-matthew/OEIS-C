/*
 * A060648  Number of cyclic subgroups of the group C_n X C_n (where C_n is
 *          the cyclic group of order n).
 * https://oeis.org/A060648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^e‖n} (p^(e+1)+p^e-2)/(p-1) (n ≥ 1)
 * Range:     u8 n <= 16, u16 n <= 952, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Simple trial‑division factorisation, sufficient for n ≤ 10⁴.
   Fills pr[0..cnt-1] and exp[0..cnt-1] with prime factors and
   their exponents.  cnt returns the number of distinct primes. */
static void
factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2)
        return;

    /* handle factor 2 */
    if ((n & 1) == 0) {
        uint64_t e = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++e;
        }
        pr[(*cnt)++] = 2;
        exp[(*cnt)-1] = e;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)++] = p;
            exp[(*cnt)-1] = e;
        }
    }
    /* remaining prime > 1 */
    if (n > 1) {
        pr[(*cnt)++] = n;
        exp[(*cnt)-1] = 1;
    }
}

/* Multiply in a type‑independent way.  seq_t may be an integer type
   or an arbitrary‑precision class; the operations +, *, / are defined. */
static inline seq_t
seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* ---- A060648 ---------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n <= 1)                /* a(1) = 1; the behaviour for n=0 is irrelevant */
        return (seq_t)1;

    uint64_t pr[32], exp[32];
    size_t cnt;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pt    = (seq_t)pr[i];
        idx_t  e    = (idx_t)exp[i];

        seq_t p_pow_e      = seq_pow(pt, e);          /* p^e   */
        seq_t p_pow_e_plus = p_pow_e * pt;            /* p^(e+1) */

        seq_t numer   = p_pow_e_plus + p_pow_e - (seq_t)2;
        seq_t denom   = pt - (seq_t)1;
        seq_t factor  = numer / denom;                /* exact division */

        result *= factor;
    }
    return result;
}
