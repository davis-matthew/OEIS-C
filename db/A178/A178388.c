/*
 * A178388  Concatenation of the first n primes written in base 3.
 * https://oeis.org/A178388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the base‑3 representations of the first
 *            n primes, interpreted as a decimal integer
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed sequence values */
    static seq_t *seq_cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cached = 0;          /* number of values already computed */

    /* static list of primes used to build the sequence */
    static uint64_t *prime_list = NULL;
    static idx_t prime_cap = 0;
    static idx_t prime_cnt = 0;       /* number of primes stored */

    /* ensure cache large enough for up to n */
    if (cache_size < n) {
        seq_cache = (seq_t*)realloc(seq_cache, n * sizeof(seq_t));
        /* new slots are zero‑initialized by realloc when expanding */
        cache_size = n;
    }

    /* generate primes until we have at least n of them */
    while (prime_cnt < n) {
        uint64_t cand;
        if (prime_cnt == 0)
            cand = 2;
        else
            cand = prime_list[prime_cnt - 1] + 1;

        /* simple primality test using already found primes */
        while (true) {
            bool ok = true;
            for (idx_t j = 0; j < prime_cnt; ++j) {
                uint64_t p = prime_list[j];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) break;
            ++cand;
        }

        /* append to prime list */
        if (prime_cnt >= prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 16;
            prime_list = (uint64_t*)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = cand;
    }

    /* compute missing terms of the sequence */
    for (idx_t i = cached; i < n; ++i) {
        uint64_t p = prime_list[i];

        /* compute base‑3 representation of p */
        uint64_t dig[64];
        int len = 0;
        uint64_t tmp = p;
        if (tmp == 0) { dig[len++] = 0; }
        while (tmp > 0) {
            dig[len++] = tmp % 3;
            tmp /= 3;
        }

        /* value of the base‑3 digits interpreted in decimal */
        seq_t part = 0;
        for (int k = len - 1; k >= 0; --k) {
            part = part * 10 + dig[k];
        }

        /* multiply previous value by 10^len and add part */
        if (i == 0) {
            seq_cache[i] = part;
        } else {
            seq_t pow10 = 1;
            for (int t = 0; t < len; ++t) pow10 *= 10;
            seq_cache[i] = seq_cache[i - 1] * pow10 + part;
        }
    }
    cached = n;

    return seq_cache[n - 1];
}
