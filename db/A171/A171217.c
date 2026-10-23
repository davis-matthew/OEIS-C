/*
 * A171217  Numbers of the form (noncomposite number)^(noncomposite number),
 *          but not p^p where p is prime.
 * https://oeis.org/A171217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers of the form (noncomposite)^(noncomposite), excluding p^p
 *            where p is prime
 * Range:     u8 n <= 56, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto, cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/cmp_seq_t.h"

static seq_t pow_limit(seq_t base, seq_t exp, seq_t limit) {
    seq_t result = 1;
    idx_t e = (idx_t)exp;
    for (idx_t i = 0; i < e; ++i) {
        if (base == 0) return 0;
        if (result > limit / base) return limit + 1;
        result *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t bound = 100;
    for (;;) {
        /* Generate primes up to the current bound */
        uint64_t *primes_arr = NULL;
        size_t primes_cnt = 0;
        build_primes_upto(bound, &primes_arr, &primes_cnt);

        /* Prepare candidate array */
        size_t list_cap = 1024;
        seq_t *cand = (seq_t *)malloc(list_cap * sizeof(seq_t));
        size_t list_len = 0;

        /* Add 1 */
        cand[list_len++] = (seq_t)1;

        /* Add primes themselves */
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (list_len == list_cap) {
                list_cap <<= 1;
                cand = (seq_t *)realloc(cand, list_cap * sizeof(seq_t));
            }
            cand[list_len++] = (seq_t)primes_arr[i];
        }

        /* Add p^q for distinct primes p,q where p^q <= bound */
        seq_t bound_seq = (seq_t)bound;
        for (size_t bi = 0; bi < primes_cnt; ++bi) {
            seq_t p = (seq_t)primes_arr[bi];
            for (size_t ei = 0; ei < primes_cnt; ++ei) {
                seq_t q = (seq_t)primes_arr[ei];
                if (q == p) continue;    /* skip p^p */
                seq_t val = pow_limit(p, q, bound_seq);
                if (val > bound_seq) break;   /* subsequent exponents grow */
                if (list_len == list_cap) {
                    list_cap <<= 1;
                    cand = (seq_t *)realloc(cand, list_cap * sizeof(seq_t));
                }
                cand[list_len++] = val;
            }
        }

        free(primes_arr);

        /* Sort candidates and remove duplicates */
        qsort(cand, list_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < list_len; ++i) {
            if (uniq == 0 || cmp_seq_t(&cand[uniq - 1], &cand[i]) != 0) {
                cand[uniq++] = cand[i];
            }
        }
        list_len = uniq;

        if (list_len >= n) {
            seq_t result = cand[n - 1];
            free(cand);
            return result;
        }
        free(cand);
        bound <<= 1;            /* double bound and try again */
    }
}
