/*
 * A107757  Numbers k such that Sum_{j=1..k} Catalan(j) == 2 (mod 3).
 * https://oeis.org/A107757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth index k for which
   Σ_{j=1..k} Catalan(j) ≡ 2 (mod 3).
   The function generates the indices on demand,
   storing them in a static cache. */
#include <stdlib.h>

/* small binomial table for base‑3 digits: 0..2 choose 0..2 modulo 3 */
static const int binom_mod3_small[3][3] = {
    {1,0,0},
    {1,1,0},
    {1,2,1}
};

/* compute binomial(n,k) modulo 3 using Lucas’ theorem */
static int binom_mod3(idx_t n, idx_t k)
{
    if (k > n) return 0;
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 3;
        int ki = k % 3;
        if (ki > ni) return 0;
        res = (res * binom_mod3_small[ni][ki]) % 3;
        n /= 3;
        k /= 3;
    }
    return res;
}

/* Catalan(n) modulo 3 via binomial difference: C(2n,n)-C(2n,n+1) */
static int catalan_mod3(idx_t n)
{
    int bin1 = binom_mod3(2*n, n);
    int bin2 = binom_mod3(2*n, n + 1);
    return (bin1 + 3 - bin2) % 3;
}

/* a(n): nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* static state across calls */
    static idx_t *cache = NULL;     /* array of found k values (idx_t) */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t curr_k = 0;        /* last processed k */
    static int curr_sum = 0;        /* cumulative sum modulo 3 up to curr_k */

    /* increase cache size if needed */
    if (cache_len < n) {
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
            if (!cache) return 0;   /* allocation failed */
        }
        while (cache_len < n) {
            ++curr_k;
            int cat = catalan_mod3(curr_k);
            curr_sum = (curr_sum + cat) % 3;
            if (curr_sum == 2) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
                    if (!tmp) {
                        free(cache);
                        return 0;   /* allocation failed */
                    }
                    cache = tmp;
                }
                cache[cache_len++] = curr_k;
            }
        }
    }
    return (seq_t)cache[n - 1];
}
