/*
 * A067941  Let rep(n) be the n-th repunit number, sequence gives values of n
 *          such that rep(n)==rep(8) (mod n).
 * https://oeis.org/A067941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th positive integer k for which 10^k ≡ 10^8 (mod
 *            9·k) i.e. the n‑th solution of rep(k) ≡ rep(8) (mod k), where
 *            rep(m) = (10^m−1)/9. The function builds the sequence lazily,
 *            caching all found terms so that subsequent calls (in any order)
 *            are answered in amortised O(1) time
 * Range:     u8 n <= 41, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of all found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;    /* how many terms are stored    */
static idx_t last_checked = 0;  /* largest k that has been tested */

/* Append a new term to the cache                           */
/*   The caller guarantees that the new term is larger     */
/*   than every value already in the cache.                 */
static void add_term(idx_t k)
{
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
    }
    cache[cache_len++] = (seq_t)k;
}

/* Modulo multiplication using 128‑bit intermediate to avoid overflow.
   Works for any idx_t that fits in 64 bits. */
static inline idx_t mul_mod(idx_t a, idx_t b, idx_t mod)
{
    return (idx_t)(((__uint128_t)a * b) % mod);
}

/* Binary modular exponentiation.
   Computes base^exp (mod mod).  The base and exponent are idx_t values.
   The result fits in idx_t. */
static idx_t pow_mod_idx(idx_t base, idx_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = mul_mod(result, b, mod);
        b = mul_mod(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

/* a(n)  --  the n-th term of A067941   */
seq_t a(idx_t n)
{
    /* generate terms until we have the n‑th one */
    while (cache_len < n) {
        ++last_checked;                                   /* next candidate k */

        idx_t mod = last_checked * 9;                     /* 9·k */
        /* 10^k  mod 9k  and  10^8  mod 9k  */
        idx_t pow_k = pow_mod_idx(10, last_checked, mod);
        idx_t pow_8 = pow_mod_idx(10, 8, mod);

        if (pow_k == pow_8)                               /* condition true */
            add_term(last_checked);
    }
    return cache[n - 1];
}
