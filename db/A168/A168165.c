/*
 * A168165  Primes which are the concatenation of a Sophie Germain prime p and
 *          2p+1.
 * https://oeis.org/A168165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for Sophie–Germain primes p and their
   safe‑prime partner q = 2p+1, concatenating p and q, testing the
   concatenated number for primality, and caching the found terms. */

#include <stdlib.h>   /* abort, malloc, realloc */
#include <stdint.h>   /* uint64_t */

/* Utility headers that provide the required functions. */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Static cache of computed terms. */
static seq_t  *terms      = NULL;  /* array of found terms           */
static size_t terms_cap   = 0;     /* current capacity of 'terms'    */
static idx_t  terms_len   = 0;     /* number of terms currently stored */
static idx_t  last_p_chk  = 1;     /* last prime examined (start at 1) */

/* Ensure the terms array can hold at least 'need' elements. */
static void
ensure_terms_capacity(size_t need)
{
    if (need <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    if (!tmp)
        abort();                       /* allocation failure */
    terms      = tmp;
    terms_cap  = new_cap;
}

/* Main sequence function: returns the n-th term (n>=1). */
seq_t
a(idx_t n)
{
    if (n == 0)                      /* offset is 1; undefined for 0 */
        return (seq_t)0;

    /* Return cached term if already available. */
    if (n <= terms_len)
        return terms[n - 1];

    /* Keep searching until we have at least n terms. */
    while (terms_len < n) {
        /* Find the next Sophie–Germain prime p. */
        for (idx_t p = last_p_chk; ; ++p) {
            if (!is_prime(p))
                continue;           /* p must be prime */

            idx_t q = 2 * p + 1;      /* safe prime candidate */
            if (!is_prime(q))
                continue;           /* q must be prime */

            /* Concatenate p and q. */
            seq_t cand = concat_parts(p, q);

            /* Test the concatenated number for primality. */
            if (is_prime_big(cand)) {
                /* Store the new term. */
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = cand;
                last_p_chk = p + 1;   /* next search starts after p */
                break;               /* move to next required term */
            }
            /* If cand is not prime, continue searching for next p. */
        }
    }

    return terms[n - 1];
}
