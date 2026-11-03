/*
 * A057660  a(n) = Sum_{k=1..n} n/gcd(n,k).
 * https://oeis.org/A057660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, mult
 * Formula:   a(n) = Sum_{d|n} d*phi(d) = Sum_{k=1..n} n/gcd(n,k) *
 *            Multiplicative form: a(p^e) = (p^(2e+1)+1)/(p+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9594 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Recursive helper: enumerate all divisors of n using its factorisation
   and accumulate d*phi(d) into *sum.
*/
static void rec_divisor_sum(size_t  idx,
                            seq_t   d,
                            seq_t   phi,
                            const idx_t *primes,
                            const int   *exps,
                            size_t       pc,
                            seq_t        *sum)
{
    if (idx == pc) {
        *sum += d * phi;
        return;
    }

    idx_t p   = primes[idx];
    int   emax = exps[idx];

    /* divisor with this prime raised to power 0 */
    rec_divisor_sum(idx + 1, d, phi, primes, exps, pc, sum);

    seq_t d_cur  = d;      /* current divisor multiplied by p^e */
    seq_t phi_cur = phi;   /* phi for current divisor */

    for (int e = 1; e <= emax; ++e) {
        d_cur = d_cur * (seq_t)p;           /* increase power of p */

        if (e == 1)
            phi_cur = phi * (seq_t)(p - 1);  /* first power of p */
        else
            phi_cur = phi_cur * (seq_t)p;    /* add another p */

        rec_divisor_sum(idx + 1, d_cur, phi_cur, primes, exps, pc, sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                    /* offset is 1, 0 is invalid */
    }

    /* --- factorise n (trial division suffices for n ≤ 10000) --- */
    idx_t n_tmp = n;
    idx_t primes[64];
    int   exps[64];
    size_t pc = 0;

    for (idx_t p = 2; p * p <= n_tmp; ++p) {
        if (n_tmp % p == 0) {
            int e = 0;
            while (n_tmp % p == 0) {
                n_tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }
    }
    if (n_tmp > 1) {
        primes[pc] = n_tmp;
        exps[pc]   = 1;
        ++pc;
    }

    /* --- enumerate divisors and sum d*phi(d) --- */
    seq_t sum = 0;
    rec_divisor_sum(0, (seq_t)1, (seq_t)1, primes, exps, pc, &sum);
    return sum;
}
