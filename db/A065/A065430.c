/*
 * A065430  Order of commutator subgroup of GL(2,Z_n) (invertible 2 X 2
 *          matrices mod n: A000252).
 * https://oeis.org/A065430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p^e∥n} f(p,e)  with  
 *   f(2,e)   = 3·2^(3e-3)                (commutator subgroup order for 2^e)  
 *   f(p,e)   = (p^2-1)·p^(3e-2)   for odd primes p  
 *   a(1) = 1
 *
 * The sequence is multiplicative; the above product gives the value for
 * arbitrary n≥1.  The implementation uses trial‑division factorisation
 * sufficient for the usual test ranges (up to a few 10^4 or higher).
 * Multiplication is performed via 128‑bit temporaries when seq_t is
 * 64‑bit to avoid overflow, otherwise it relies on the natural
 * multiplication of seq_t (e.g. big integers).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple trial‑division factorisation of an unsigned 64‑bit integer.
   The result is stored in the arrays `pr` and `exp` of length `cap`,
   and `cnt` gives the number of distinct prime factors found. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    if (n < 2) {
        *cnt = c;
        return;
    }
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* handle offset n=1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = (seq_t)1;

        if (p == (uint64_t)2) {            /* power of two */
            /* 3 * 2^(3e-3) */
            uint64_t exp2 = 3 * e - 3;
            seq_t pow2 = (seq_t)1;
            for (uint64_t k = 0; k < exp2; ++k)
                pow2 = (seq_t)((__uint128_t)pow2 * (seq_t)2);
            term = (seq_t)((__uint128_t)3 * pow2);
        } else {                            /* odd prime power */
            seq_t p_sq_minus1 = (seq_t)(p * p) - (seq_t)1;  /* p^2 - 1 */
            uint64_t expp = 3 * e - 2;
            seq_t powp = (seq_t)1;
            for (uint64_t k = 0; k < expp; ++k)
                powp = (seq_t)((__uint128_t)powp * (seq_t)p);
            term = (seq_t)((__uint128_t)p_sq_minus1 * powp);
        }

        result = (seq_t)((__uint128_t)result * term);
    }

    return result;
}
