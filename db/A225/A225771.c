/*
 * A225771  Numbers that are positive integer divisors of 1 + 2*x^2 where x is
 *          a positive integer.
 * https://oeis.org/A225771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose prime factors are congruent to 1 or 3 (mod 8). The
 *            nth term is found by sieving up to a bound that is increased
 *            until enough terms are accumulated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Below the offset a(1)=1, the sequence is undefined; return 0. */
    if (n == 0ULL) return 0;

    /* 0‑based target index */
    idx_t target = n - 1;

    /* Initial upper bound guess – sufficient for n≈10 000. */
    size_t limit = (size_t)( (size_t)n * 40ULL );
    if (limit < 200) limit = 200;

    while (1) {
        /* Allocate boolean arrays.  Cast is required for C++. */
        unsigned char *is_prime = (unsigned char*)malloc(limit + 1);
        unsigned char *good     = (unsigned char*)malloc(limit + 1);
        if (!is_prime || !good) {
            free(is_prime);
            free(good);
            return 0;   /* allocation failure – unlikely for typical n */
        }

        /* --- Sieve of Eratosthenes for primes up to limit --- */
        is_prime[0] = 0;
        is_prime[1] = 0;
        for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;

        for (size_t p = 2; p * p <= limit; ++p) {
            if (!is_prime[p]) continue;
            for (size_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = 0;
        }

        /* --- Mark numbers that are allowed: keep only 1‑ or 3‑mod‑8 primes --- */
        good[0] = 0;
        good[1] = 1;                /* 1 has no prime factors, so it is good */
        for (size_t i = 2; i <= limit; ++i) good[i] = 1;

        for (size_t p = 2; p <= limit; ++p) {
            if (!is_prime[p]) continue;
            /* Exclude 2 and primes ≡5 or 7 (mod 8).  Keep 1 and 3 (mod 8). */
            if (p == 2 || (p % 8 == 5) || (p % 8 == 7)) {
                for (size_t v = p; v <= limit; v += p)
                    good[v] = 0;
            }
        }

        /* --- Count the good numbers to reach the target --- */
        idx_t cnt = 0;
        for (size_t i = 1; i <= limit; ++i) {
            if (good[i]) {
                if (cnt == target) {
                    seq_t res = (seq_t)i;
                    free(is_prime);
                    free(good);
                    return res;
                }
                ++cnt;
            }
        }

        /* Not enough terms: increase bound and try again. */
        free(is_prime);
        free(good);
        limit <<= 1;            /* double the limit */
    }
}
