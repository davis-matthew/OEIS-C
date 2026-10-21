/*
 * A069094  Jordan function J_9(n).
 * https://oeis.org/A069094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(9e) – p^(9(e‑1))) – Jordan totient function
 *            J_9(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute base^exp into *res, exp is an idx_t */
static void pow_seq_t(seq_t base, idx_t exp, seq_t *res)
{
    *res = (seq_t)1;
    while (exp) {
        *res *= base;
        exp--;
    }
}

/* Simple trial‑division factorisation into primes[] / exps[], count in *cnt.
   n fits in uint64_t, primes[] large enough. */
static void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    while ((n & 1u) == 0) {
        primes[*cnt] = 2;
        exps[*cnt] = 0;
        if (*cnt == 0) {
            (*cnt)++;                  /* start count for 2 */
        }
        exps[*cnt-1]++;              /* increase exponent for 2 */
        n >>= 1;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            primes[*cnt] = d;
            exps[*cnt] = 0;
            while (n % d == 0) {
                exps[*cnt]++;
                n /= d;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        primes[*cnt] = n;
        exps[*cnt] = 1;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; i++) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t p_pow_92e, p_pow_9e_minus9;
        pow_seq_t((seq_t)p, 9 * e, &p_pow_92e);
        if (e == 0) {
            /* e = 0 cannot happen for a prime power factor, but guard */
            p_pow_9e_minus9 = (seq_t)1;
        } else {
            pow_seq_t((seq_t)p, 9 * (e - 1), &p_pow_9e_minus9);
        }

        seq_t term = p_pow_92e - p_pow_9e_minus9;  /* a(p^e) */
        result *= term;
    }
    return result;
}
