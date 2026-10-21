/*
 * A057256  Numbers k such that k | 6^k + 5^k + 4^k + 3^k + 2^k.
 * https://oeis.org/A057256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that k | 6^k + 5^k + 4^k + 3^k +
 *            2^k
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* modular exponentiation:  base^exp mod mod  (64‑bit operands) */
static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1)
        return 0;                     /* any number ≡ 0 (mod 1) */
    uint64_t result = 1 % mod;
    uint64_t a = base % mod;
    while (exp > 0)
    {
        if (exp & 1U)
            result = (unsigned __int128)result * a % mod;
        a = (unsigned __int128)a * a % mod;
        exp >>= 1U;
    }
    return result;
}

/* sequence A057256 */
seq_t a(idx_t n)
{
    /* static on‑call cache */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_k    = 1;      /* next integer to test */

    while (cache_len < (size_t)n)
    {
        idx_t k   = next_k;
        uint64_t mod = (uint64_t)k;
        uint64_t sum = 0;

        /* sum_{b=2}^{6} b^k (mod k) */
        for (uint64_t base = 2; base <= 6; ++base)
        {
            uint64_t p = mod_pow_u64(base, k, mod);
            sum += p;
            if (sum >= mod)
                sum -= mod;          /* keep sum modulo k */
        }

        if (sum % mod == 0)          /* k satisfies the condition */
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 128 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;  /* allocation failure – undefined behaviour */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
