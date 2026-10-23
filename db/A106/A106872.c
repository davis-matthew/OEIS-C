/*
 * A106872  Primes of the form 2x^2+xy+4y^2.
 * https://oeis.org/A106872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as 2x^2 + xy + 4y^2
 * Range:     u8 n <= 19, u16 n <= 2197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/modpow_uint64.h"   /* modpow_uint64(unsigned long long base,
                                     unsigned long long exp,
                                     unsigned long long mod) */
#include "../utils/isPrime64.h"     /* isPrime64(uint64_t) */

/* Helper: check whether prime p can be expressed as 2x^2 + xy + 4y^2 */
static bool prime_represents(uint64_t p)
{
    if (p == 2)                     /* 2 = 2·1^2 + 1·0 + 4·0^2 */
        return true;

    /* Quick quadratic‑residue test for discriminant –31 */
    if (p % 2 == 0)                 /* primes >2 are odd */
        return false;
    if (p == 31)                    /* 31 is prime but not represented */
        return false;

    uint64_t d = (p - (31 % p)) % p;   /* d ≡ –31 (mod p) */
    if (d == 0)                        /* p divides 31 → p = 31 */
        return false;
    uint64_t leg = modpow_uint64(d, (p - 1) / 2, p);
    if (leg != 1)                      /* not a quadratic residue → cannot be represented */
        return false;

    /* Exact representation test by solving 2x² + xy + (4y² – p) = 0 */
    unsigned __int128 total = (unsigned __int128)8 * p;
    for (uint64_t y = 0; ; ++y) {
        unsigned __int128 penalty = (unsigned __int128)31 * y * y;
        if (penalty > total)
            break;
        unsigned __int128 d128 = total - penalty;
        uint64_t disc = (uint64_t)d128;           /* discriminant 8p – 31y² is small enough for 64‑bit */

        uint64_t t = (uint64_t)(sqrt((long double)disc) + 0.5);
        if (t * t != disc)
            continue;

        /* x = (–y ± t) / 4 must be integral */
        if (((t - y) & 3) == 0 || ((t + y) & 3) == 0)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static idx_t computed = 0;          /* how many terms already found */
    static seq_t *terms  = NULL;        /* array of found primes */
    static size_t cap    = 0;           /* current capacity of the array */
    static uint64_t cand   = 2;         /* current candidate integer */

    /* Grow the array if necessary */
    if (computed == 0 && terms == NULL) {
        cap = 8;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (computed < n) {
        /* Skip to next odd candidate after 2 */
        if (cand == 2) {                 /* first prime */
            /* already checked below */
        } else {
            if ((cand & 1) == 0)
                cand++;
        }

        if (isPrime64(cand) && prime_represents(cand)) {
            if (computed == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) abort();       /* allocation failure */
                terms = tmp;
            }
            terms[computed++] = (seq_t)cand;
        }

        cand += 2;                       /* only odd candidates needed */
    }

    return terms[n - 1];
}
