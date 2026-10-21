/*
 * A095242  a(n) = smallest positive number such that concatenation
 *          a(1)...a(n) is divisible by n-th prime.
 * https://oeis.org/A095242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 499, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095242 : a(n) = smallest positive integer such that
   the concatenation a(1)...a(n) is divisible by the n‑th prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* return number of decimal digits of x (x > 0) */
static int digits_seq(seq_t x)
{
    int d = 0;
    while (x > 0) {
        x = x / 10;
        ++d;
    }
    return d ? d : 1;
}

/* sequence entry a(n) */
seq_t a(idx_t n)
{
    static bool inited = false;
    static seq_t *seq = NULL;          /* stored a(1)..a(k)    */
    static size_t seq_len = 0;         /* number of stored terms */
    static size_t seq_cap = 0;         /* capacity of seq array */
    static idx_t *primes = NULL;       /* list of primes       */
    static size_t prime_cnt = 0;       /* number of primes     */

    /* one‑time initialization */
    if (!inited) {
        /* build enough primes; prime(10000)≈104729, 200000 is safe */
        size_t prime_limit = 200000;
        build_primes((idx_t)prime_limit, &primes, &prime_cnt);

        /* initial capacity for a(1)..a(k)  */
        seq_cap = 8192;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        inited = true;
    }

    /* grow sequence array if necessary */
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (seq_len < (size_t)n) {
        /* prime for this step (0‑based index) */
        idx_t idx = (idx_t)seq_len;
        seq_t p = (seq_t)primes[idx];

        /* remainder of concatenation a(1)..a(seq_len) modulo p */
        seq_t rem = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t val = seq[i];
            int d = digits_seq(val);

            /* compute 10^d mod p */
            seq_t pow10 = 1;
            for (int j = 0; j < d; ++j) {
                pow10 = (pow10 * 10) % p;
            }

            rem = (rem * pow10 + (val % p)) % p;
        }

        /* search for smallest k such that
           (rem * 10^d + k) ≡ 0 (mod p) with k having d digits */
        int d = 1;
        seq_t pow10mod = 10 % p;            /* 10^1 mod p */
        seq_t lower = 1;                    /* 10^{d-1} */
        seq_t upper = 9;                    /* 10^d - 1 */

        seq_t value = 0;
        while (1) {
            seq_t prod = (rem * pow10mod) % p;          /* rem*10^d mod p */
            seq_t target = (p - prod) % p;              /* desired k mod p */

            seq_t t;
            if (target < lower) {
                /* t = ceil((lower - target)/p) */
                t = ((lower - target) + p - 1) / p;
            } else {
                t = 0;
            }
            value = target + t * p;                     /* candidate k */

            /* check if value has exactly d digits */
            if (digits_seq(value) == d) {
                seq[seq_len++] = value;
                break;
            }

            /* increase digit length */
            ++d;
            lower *= 10;
            upper = lower * 10 - 1;
            pow10mod = (pow10mod * 10) % p;
        }
    }

    return seq[n - 1];
}
