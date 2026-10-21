/*
 * A057492  Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n + 6^n + 5^n
 *          + 4^n.
 * https://oeis.org/A057492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n + 6^n + 5^n + 4^n */
/* This function returns the n-th such integer, i.e., a(n). */
#include <stdlib.h>
#include <stdint.h>

/* helper: modular exponentiation (base^exp mod mod) */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t checked_up_to = 0;   /* last integer checked */

    /* generate terms until we have at least n of them */
    while (terms_len < n)
    {
        ++checked_up_to;
        uint64_t m = checked_up_to;
        uint64_t sum = 0;
        for (uint64_t k = 4; k <= 11; ++k)
            sum = (sum + powmod64(k, m, m)) % m;
        if (sum == 0)
        {
            /* grow cache if needed */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) return 0; /* allocation failure, return 0 as fallback */
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
    }
    return terms[n - 1];
}
