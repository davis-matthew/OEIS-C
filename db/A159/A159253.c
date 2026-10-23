/*
 * A159253  a(n) is the smallest positive integer not yet in the sequence such
 *          that n * a(n) is a cube.
 * https://oeis.org/A159253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) is the smallest positive integer not yet used such that n *
 *            a(n) is a perfect cube (OEIS A159253). The implementation
 *            computes the sequence on demand, caching results so that
 *            subsequent calls are O(1). It evaluates the cube condition by
 *            using the prime‑factor decomposition of n and requires...
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

/* Computes a(n) for n >= 1 using lazy evaluation and memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1, but guard */

    /* ------------------------------------------------------------------ */
    /*  Static state maintained between calls                            */
    /* ------------------------------------------------------------------ */
    static idx_t max_computed        = 0;          /* highest index computed */
    static seq_t *seq_cache          = NULL;        /* array of computed a(i) */
    static size_t seq_cap            = 0;           /* allocated capacity for seq_cache */

    static seq_t *used_numbers       = NULL;        /* array holding all a(i) values so far */
    static size_t used_cap           = 0;           /* allocated capacity for used_numbers */
    static idx_t used_size           = 0;           /* number of entries in used_numbers */

    static uint64_t *primes          = NULL;        /* list of primes up to current limit */
    static size_t primes_count       = 0;
    static idx_t primes_limit        = 0;           /* current maximum prime needed */

    /* ------------------------------------------------------------------ */
    /*  Prepare prime list up to at least n                              */
    /* ------------------------------------------------------------------ */
    if (primes_limit < n) {
        free(primes);                              /* discard old list if any */
        build_primes_upto(n, &primes, &primes_count);
        primes_limit = n;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute all missing terms up to n                                 */
    /* ------------------------------------------------------------------ */
    while (max_computed < n) {
        idx_t       m   = max_computed + 1;         /* next index to compute   */
        idx_t       temp = m;                       /* copy of m for factorisation */
        seq_t       base = (seq_t)1;                /* minimal value that makes
                                                       n*a(n) a cube          */

        /* --- Prime factorisation of m to compute the base value ----- */
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > (uint64_t)temp) break;

            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp) {
                uint64_t need = (3 - (exp % 3)) % 3;      /* exponent required in base */
                if (need) {
                    for (uint64_t k = 0; k < need; ++k)
                        base *= (seq_t)p;
                }
            }
        }

        if (temp > 1) {                    /* remaining prime factor */
            uint64_t p   = temp;
            uint64_t need = (3 - (1 % 3)) % 3;
            if (need) {
                for (uint64_t k = 0; k < need; ++k)
                    base *= (seq_t)p;
            }
        }

        /* --- Find smallest unused value of the form base * t^3 ------- */
        idx_t  t   = 1;
        seq_t val  = 0;
        int   used_flag;
        for (;;) {
            val = (seq_t)((__int128)base * t * t * t);
            used_flag = 0;
            for (idx_t i = 0; i < used_size; ++i) {
                if (used_numbers[i] == val) { used_flag = 1; break; }
            }
            if (!used_flag) break;
            ++t;
        }

        /* --- Store the new term ------------------------------------- */
        ensure_capacity(max_computed + 1, &seq_cap,
                        (void **)&seq_cache, sizeof(seq_t));
        seq_cache[max_computed] = val;

        ensure_capacity(used_size + 1, &used_cap,
                        (void **)&used_numbers, sizeof(seq_t));
        used_numbers[used_size] = val;

        ++used_size;
        ++max_computed;
    }

    /* indices in seq_cache are zero‑based */
    return seq_cache[n - 1];
}
