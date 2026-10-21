/*
 * A067943  Let rep(n) be the n-th repunit number, sequence gives values of n
 *          such that rep(n)==rep(10) (mod n).
 * https://oeis.org/A067943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067943
    a(n) is the n-th positive integer m such that
    10^m ≡ 10^10 (mod 9·m),
    which is equivalent to rep(m) ≡ rep(10) (mod m). */

#include <stdint.h>
#include <stdbool.h>

/*  modular multiplication using 128‑bit intermediate */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((__uint128_t)a * b) % mod);
}

/*  fast exponentiation: computes base^exp mod mod  (base = 10)   */
static seq_t pow_mod_10(idx_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    seq_t base = 10 % mod;
    while (exp) {
        if (exp & 1)
            res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/*  main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    idx_t count = 0, candidate = 1;
    for (;;) {
        seq_t mod = (seq_t)9 * (seq_t)candidate;        /* 9·candidate */

        /* compare 10^candidate with 10^10 modulo 9·candidate */
        if (pow_mod_10(candidate, mod) == pow_mod_10(10, mod)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
