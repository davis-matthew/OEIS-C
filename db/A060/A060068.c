/*
 * A060068  Divide n! by largest power of n which will leave the result an
 *          integer.
 * https://oeis.org/A060068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / n^A011776(n) (greatest integer divisor n! / n^k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1, but harmless */

    /* Special case n = 1 */
    if (n == 1) return (seq_t)1;

    /* --- Prime table: build or enlarge up to n --------------------------------- */
    static idx_t prime_limit = 0;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (n > prime_limit) {
        if (primes) free(primes);   /* free old table if any */
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    /* --- Factor n: store distinct primes of n and their exponents ----------------- */
    static uint64_t *npr = NULL;
    static unsigned *ne = NULL;
    static size_t nat_size = 0;

    if (nat_size < prime_cnt) { /* Ensure enough space for worst case */
        if (npr) free(npr);
        if (ne)  free(ne);
        nat_size = prime_cnt;
        npr = (uint64_t*)malloc(nat_size * sizeof(uint64_t));
        ne  = (unsigned*)malloc(nat_size * sizeof(unsigned));
    }

    size_t nf = 0;                    /* number of distinct primes of n */
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; i++) {
        if (tmp % primes[i] == 0) {
            unsigned e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                e++;
            }
            npr[nf] = primes[i];
            ne[nf]  = e;
            nf++;
        }
    }
    if (tmp > 1) {                     /* remaining prime factor */
        npr[nf] = tmp;
        ne[nf]  = 1;
        nf++;
    }

    /* --- Determine e = min_p floor(vp(n!)/exp_in_n) -------------------------------- */
    idx_t e = n;                       /* upper bound for the exponent */
    for (size_t i = 0; i < nf; i++) {
        uint64_t p = npr[i];
        unsigned exp_in_n = ne[i];

        /* Compute vp(n!) for this prime */
        idx_t vp_fact = 0;
        idx_t div = p;
        while (div <= n) {
            vp_fact += n / div;
            if (div > n / p) break;   /* prevent overflow */
            div *= p;
        }

        idx_t cand = vp_fact / exp_in_n;
        if (cand < e) e = cand;
    }

    /* --- Compute result as product over all primes <= n of p^(vp(n!) - e*exp_in_n) ----- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; i++) {
        uint64_t p = primes[i];

        /* vp(n!) for this prime */
        idx_t vp_fact = 0;
        idx_t div = p;
        while (div <= n) {
            vp_fact += n / div;
            if (div > n / p) break;
            div *= p;
        }

        /* Subtract e * exp_in_n if p divides n */
        idx_t sub = 0;
        for (size_t j = 0; j < nf; j++) {
            if (npr[j] == p) {
                sub = (idx_t)ne[j] * e;
                break;
            }
        }

        idx_t exp = vp_fact - sub;
        if (exp > 0) {
            seq_t pw = big_pow((seq_t)p, (seq_t)exp);
            result = seq_mul(result, pw);
        }
    }

    return result;
}
