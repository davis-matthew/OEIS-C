/*
 * A175210  a(n) = largest prime (or 1, if no such prime exists) that is <
 *          p(n) and whose binary representation occurs as a substring in the
 *          binary representation of p(n), where p(n) is the n-th prime.
 * https://oeis.org/A175210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest prime < p(n) whose binary representation appears
 *            in the binary representation of p(n). The function computes the
 *            n‑th term of OEIS sequence A175210
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Find the n‑th term of A175210.  n is 1‑based. */
seq_t
a(idx_t n)
{
    if (n <= 2) return (seq_t)1;          /* a(1) = a(2) = 1 */

    /* ------------------------------------------------------------------
       Lazily build (and grow) a prime table large enough for the
       requested index.  We use a very generous upper bound for the
       n-th prime:  for n ≥ 6,  p(n) < n( ln n + ln ln n ) < 20·n
       (safe for n ≤ 10⁶, and far larger than required for n ≤ 10⁴).
       ------------------------------------------------------------------ */
    static uint64_t *primes = NULL;
    static size_t  prime_cnt = 0;     /* number of primes actually stored */

    /* Ensure we have at least n primes. */
    if (prime_cnt < (size_t)n) {
        /* Compute a safe limit for the n-th prime. */
        idx_t limit;
        if (n < 6) limit = 15;
        else limit = (idx_t)(n * 20ULL);   /* very conservative estimate */

        /* Allocate a new array. */
        uint64_t *tmp = NULL;
        size_t   cnt  = 0;
        build_primes_upto(limit, &tmp, &cnt);

        /* Free old array (if any) and replace. */
        free(primes);
        primes = tmp;
        prime_cnt = cnt;
    }

    /* p(n) is the n‑th prime (1‑based). */
    uint64_t p = primes[(size_t)(n - 1)];

    /* ------------------------------------------------------------------
       Convert p to its binary representation as a C string (MSB first).
       ------------------------------------------------------------------ */
    char p_bin[128];
    int  p_len = 0;
    uint64_t t = p;
    while (t) {
        p_bin[p_len++] = (t & 1ULL) ? '1' : '0';
        t >>= 1;
    }
    if (p_len == 0) p_bin[p_len++] = '0';    /* only for p == 0, not used */
    /* Reverse to MSB first. */
    for (int i = 0; i < p_len / 2; ++i) {
        char c = p_bin[i];
        p_bin[i] = p_bin[p_len - 1 - i];
        p_bin[p_len - 1 - i] = c;
    }
    p_bin[p_len] = '\0';

    /* Search primes smaller than p(n) from largest downward. */
    uint64_t best = 1;            /* default if nothing found */

    for (idx_t i = n - 2; ; --i) {
        uint64_t q = primes[(size_t)i];

        /* Convert q to binary string. */
        char q_bin[128];
        int  q_len = 0;
        uint64_t tt = q;
        while (tt) {
            q_bin[q_len++] = (tt & 1ULL) ? '1' : '0';
            tt >>= 1;
        }
        if (q_len == 0) q_bin[q_len++] = '0';
        for (int j = 0; j < q_len / 2; ++j) {
            char c = q_bin[j];
            q_bin[j] = q_bin[q_len - 1 - j];
            q_bin[q_len - 1 - j] = c;
        }
        q_bin[q_len] = '\0';

        /* Check if q_bin appears inside p_bin. */
        if (strstr(p_bin, q_bin) != NULL) {
            best = q;
            break;            /* largest such prime found */
        }

        if (i == 0) break;      /* prevent underflow of unsigned idx_t */
    }

    return (seq_t)best;
}
