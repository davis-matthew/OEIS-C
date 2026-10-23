/*
 * A014953  Numbers k such that k divides s(k), where s(1)=1, s(j) = s(j-1) +
 *          j*13^(j-1).
 * https://oeis.org/A014953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer k such that k divides s(k),
 *            where s(1)=1, s(j)=s(j-1)+j*13^(j-1). Equivalently 144*k |
 *            1-(1-12*k)*13^k. This implementation generates the sequence on
 *            demand by testing integers in increasing order. It only uses
 *            integer arithmetic and memoises already-found terms s...
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

static bool is_valid(idx_t k)
{
    if (k == 1)
        return true;

    uint64_t k64 = (uint64_t)k;
    uint64_t mod = 144ULL * k64;              /* 144*k          */
    uint64_t t   = pow_mod_uint64(13ULL, k64, mod); /* 13^k mod 144*k */

    /* u = (1 - 12*k) mod mod */
    uint64_t u = (1ULL + mod - (12ULL * k64 % mod)) % mod;

    /* val = (1 - u*t) mod mod */
    uint64_t val = (1ULL + mod - (u * t % mod)) % mod;

    return val == 0;
}

seq_t a(idx_t n)
{
    /* Static table to cache computed sequence values */
    static seq_t *terms = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next_candidate = 1;

    while (len < (size_t)n) {
        /* Search for the next valid k */
        for (idx_t k = next_candidate; ; ++k) {
            if (!is_valid(k))
                continue;

            /* Grow capacity if needed */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                terms = (seq_t *)realloc((void *)terms, new_cap * sizeof(seq_t));
                cap = new_cap;
            }

            terms[len++] = (seq_t)k;
            next_candidate = k + 1;   /* next search starts after current k */
            break;                   /* found next term, continue outer loop */
        }
    }

    return terms[n - 1];
}
