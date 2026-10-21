/*
 * A063877  Composite numbers not divisible by 7 which in base 7 contain their
 *          largest proper factor as a substring.
 * https://oeis.org/A063877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd_uint64, is_probable_prime, modpow, mulmod, pollard,
 *            smallest_prime_factor, to_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth composite number not divisible by 7
 * whose base–7 representation contains its largest proper divisor
 * as a contiguous substring.  The value is found by a search over
 * increasing integers, rejecting primes, numbers divisible by 7,
 * and testing the substring property.  Factorisation uses a
 * deterministic Miller–Rabin test for primality and a Pollard–Rho
 * algorithm to obtain the smallest prime factor (from which the
 * largest proper factor is derived).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/is_probable_prime.h"
#include "../utils/modpow.h"
#include "../utils/mulmod.h"
#include "../utils/pollard.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/to_base7.h"

/* small auxiliary arithmetic helpers */

/* dynamic array to hold computed terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 4;   /* start checking from 4 */

/* ensure dynamic array has room for at least 'need' elements */
static void ensure_capacity(size_t need) {
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed */
    terms = tmp;
    terms_cap = new_cap;
}

/* push a new term into the dynamic array */
static void push_term(seq_t val) {
    if (terms_len == terms_cap) {
        ensure_capacity(terms_len + 1);
    }
    terms[terms_len++] = val;
}

/* main function requested by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */
    /* generate terms until we have at least n */
    while (terms_len < (size_t)n) {
        /* find next candidate */
        uint64_t cand = next_candidate++;
        if (cand % 7 == 0) continue;                 /* skip multiples of 7 */
        if (is_probable_prime(cand)) continue;        /* skip primes */

        uint64_t spf = smallest_prime_factor(cand);  /* smallest prime factor */
        uint64_t largest = cand / spf;               /* largest proper factor */

        /* check substring condition in base 7 */
        char numbuf[32], largbuf[32];
        to_base7(cand, numbuf);
        to_base7(largest, largbuf);
        if (strstr(numbuf, largbuf) != NULL) {
            push_term((seq_t)cand);
        }
    }
    return terms[n - 1];
}
