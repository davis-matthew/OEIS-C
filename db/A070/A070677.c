/*
 * A070677  Smallest m in range 1..phi(n) such that 5^m == 1 mod n, or 0 if no
 *          such number exists.
 * https://oeis.org/A070677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ∈ [1..φ(n)] such that 5^m ≡ 1 (mod n), 0 if no
 *            such m exists (e.g. n is a multiple of 5 or n = 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t,uint64_t*,uint64_t*,size_t*); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t,uint64_t,seq_t); */

seq_t a(idx_t n)
{
    /* special cases ----------------------------------------------------- */
    if (n == 1)                 return (seq_t)0;
    if (n % 5 == 0)             return (seq_t)0;  /* 5 not invertible mod n */

    /* factor n to compute φ(n) ---------------------------------------- */
    uint64_t n_pr[64], n_ex[64];
    size_t n_cnt;
    factor_u64((uint64_t)n, n_pr, n_ex, &n_cnt);

    uint64_t phi_val = (uint64_t)n;
    for (size_t i = 0; i < n_cnt; ++i)
        phi_val = phi_val / n_pr[i] * (n_pr[i] - 1);

    /* factor φ(n) ------------------------------------------------------ */
    uint64_t phi_pr[64], phi_ex[64];
    size_t phi_cnt;
    factor_u64(phi_val, phi_pr, phi_ex, &phi_cnt);

    /* multiplicative order of 5 modulo n ------------------------------- */
    seq_t ord  = (seq_t)phi_val;          /* current candidate order   */
    seq_t mod  = (seq_t)n;                /* modulus                   */

    for (size_t i = 0; i < phi_cnt; ++i) {
        seq_t p = (seq_t)phi_pr[i];
        /* try to reduce ord by p as many times as possible */
        while (ord % p == (seq_t)0) {
            seq_t cand = ord / p;
            if (mod_pow((seq_t)5, (uint64_t)cand, mod) == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}
