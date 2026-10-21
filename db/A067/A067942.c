/*
 * A067942  Let rep(n) be the n-th repunit number; sequence gives values of n
 *          such that rep(n) == rep(9) (mod n).
 * https://oeis.org/A067942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth positive integer k such that * ((10^k – 1)/9) ≡
 *            ((10^9 – 1)/9) (mod k). * * The function builds the sequence on
 *            demand and caches the results * so that subsequent calls are
 *            answered in O(1). The test of a * candidate k uses modular
 *            arithmetic with the modulus 9*k; no * large interm...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*------ Internal helper: modular exponentiation of 10^exp (mod mod) ------*/
static unsigned long long pow10mod(unsigned long long exp, unsigned long long mod)
{
    unsigned long long res = 1 % mod;
    unsigned long long base = 10 % mod;

    while (exp) {
        if (exp & 1U)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/*------ Main sequence function ------*/
seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next candidate k to test */
    static idx_t next_candidate = 1;

    /* Helper to grow the cache */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= cache_len)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        unsigned long long k = (unsigned long long)next_candidate;
        unsigned long long mod = 9ULL * k;               /* modulus 9k */

        unsigned long long t = pow10mod(k, mod);         /* 10^k mod 9k */
        t = (t + mod - 1ULL) % mod;                     /* (10^k - 1) mod 9k */
        unsigned long long rep_k = t / 9ULL;             /* rep(k) mod k; 0 ≤ rep_k < k */

        unsigned long long rep9_mod = 111111111ULL % k;  /* rep(9) mod k */

        if (rep_k == rep9_mod) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap <= cache_len)
                    new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
