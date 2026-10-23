/*
 * A110353  Value of k pertaining to A110351(n). Least k such that the sum
 *          (n+1) + (n+2) + ...+(n+k) is a multiple of the n-th triangular
 *          number n(n+1)/2.
 * https://oeis.org/A110353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that (n+1)+(n+2)+…+(n+k) is a multiple
 *            of the n‑th triangular number n(n+1)/2. The condition is
 *            equivalent to n | k·(k+1) and (n+1) | k·(k-1). For each prime
 *            power p^e dividing n, we must have k ≡ 0 or k ≡ –1 (mod p^e).
 *            For each prime power q^f dividing n+1, we must have k...
 * Range:     u8 n <= 15, u16 n <= 287, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_prime_powers, inv64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_prime_powers.h"
#include "../utils/inv64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* factorisations of n and n+1 */
    uint64_t pows_n[20], pows_np1[20];
    size_t cnt_n  = factor_prime_powers((uint64_t)n,      pows_n);
    size_t cnt_np1 = factor_prime_powers((uint64_t)(n+1), pows_np1);

    seq_t best_k = 0;   /* 0 indicates that no value has been found yet */

    uint64_t masks_n   = 1ULL << cnt_n;
    uint64_t masks_np1 = 1ULL << cnt_np1;

    for (uint64_t m_n = 0; m_n < masks_n; ++m_n) {
        for (uint64_t m_np1 = 0; m_np1 < masks_np1; ++m_np1) {
            seq_t res  = 0;          /* current residue */
            seq_t mod  = 1;          /* current modulus */

            /* process prime powers of n: k ≡ 0 or –1 (mod p) */
            for (size_t i = 0; i < cnt_n; ++i) {
                uint64_t p = pows_n[i];
                seq_t r;               /* desired residue modulo p */
                if (m_n & (1ULL << i)) r = (seq_t)(p - 1); /* –1 */
                else r = 0;
                /* merge: x ≡ r (mod p) with x ≡ res (mod mod) */
                uint64_t mod_p = (uint64_t)(mod % (seq_t)p);
                uint64_t reg = (uint64_t)(res % (seq_t)p);
                uint64_t t = ((uint64_t)r + p - reg) % p;   /* (r - res) mod p */
                uint64_t inv = inv64(mod_p, p);
                t = (t * inv) % p;
                seq_t t_seq = (seq_t)t;
                seq_t new_res = res + mod * t_seq;
                seq_t new_mod = mod * (seq_t)p;
                res = new_res;
                mod = new_mod;
            }

            /* process prime powers of n+1: k ≡ 0 or 1 (mod q) */
            for (size_t i = 0; i < cnt_np1; ++i) {
                uint64_t p = pows_np1[i];
                seq_t r;
                if (m_np1 & (1ULL << i)) r = 1;
                else r = 0;
                uint64_t mod_p = (uint64_t)(mod % (seq_t)p);
                uint64_t reg = (uint64_t)(res % (seq_t)p);
                uint64_t t = ((uint64_t)r + p - reg) % p;
                uint64_t inv = inv64(mod_p, p);
                t = (t * inv) % p;
                seq_t t_seq = (seq_t)t;
                seq_t new_res = res + mod * t_seq;
                seq_t new_mod = mod * (seq_t)p;
                res = new_res;
                mod = new_mod;
            }

            seq_t k_candidate = res % mod;
            if (k_candidate == 0) k_candidate = mod;   /* replace 0 by modulus */

            if (best_k == 0 || k_candidate < best_k)
                best_k = k_candidate;
        }
    }
    return best_k;
}
