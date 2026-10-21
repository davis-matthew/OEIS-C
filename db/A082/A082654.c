/*
 * A082654  Order of 4 mod n-th prime: least k such that prime(n) divides
 *          4^k-1, n >= 2.
 * https://oeis.org/A082654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, if n = 1; otherwise a(n) = order of 4 modulo prime(n)
 *            (least k ≥ 1 such that 4^k ≡ 1 (mod prime(n)))
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cmp_seq_t, factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- required helpers from utils ---------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k);           */
#include "../utils/factorize.h"  /* size_t factorize(uint64_t n, uint64_t *p, int *e); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/cmp_seq_t.h"  /* int cmp_seq_t(const void*, const void*); */

/* --- helper: enumerate all divisors of a number -------------------------- */
static void
gen_divs(int idx, seq_t cur, uint64_t *primes, int *exps, int cnt,
         seq_t *out, size_t *outcnt)
{
    if (idx == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    seq_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, cur * val, primes, exps, cnt, out, outcnt);
        val *= primes[idx];
    }
}

/* --- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* nth prime (n ≥ 2) */
    uint32_t p32 = get_prime(n);
    uint64_t p   = (uint64_t)p32;

    /* φ(p) = p - 1 */
    uint64_t phi = p - 1;

    /* factor φ */
    uint64_t fac[64];
    int      exp[64];
    size_t   faccnt = factorize(phi, fac, exp);

    /* generate all divisors of φ */
    seq_t divs[2048];   /* enough for φ < 2^32 */
    size_t dcnt = 0;
    gen_divs(0, 1, fac, exp, (int)faccnt, divs, &dcnt);

    /* sort divisors ascending */
    qsort(divs, dcnt, sizeof(seq_t), cmp_seq_t);

    /* find the smallest divisor d with 4^d ≡ 1 (mod p) */
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divs[i];
        if (mod_pow((seq_t)4, (uint64_t)d, (seq_t)p) == (seq_t)1)
            return d;
    }

    /* for odd primes this should never happen */
    return (seq_t)0;
}
