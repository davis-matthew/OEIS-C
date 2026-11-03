/*
 * A157676  Numbers n such that n + (product of digits of n) is prime.
 * https://oeis.org/A157676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer m such that m + digit_product(m)
 *            is prime
 * Range:     u8 n <= 16, u16 n <= 4699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 7493 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is built on demand; every call returns the nth term without assuming increasing order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n) {
    static seq_t *terms = NULL;     /* array of generated terms */
    static size_t terms_cap = 0;    /* allocated capacity */
    static size_t terms_len = 0;    /* number of generated terms */
    static idx_t candidate = 1;     /* next integer to test */

    /* generate terms until we reach the requested index */
    while (terms_len < n) {
        seq_t cand = (seq_t)candidate;
        seq_t prod = digit_product(cand);
        seq_t sum  = cand + prod;            /* m + product of digits */

        if (is_prime_big(sum)) {
            /* ensure capacity and store the term */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) return 0;            /* abort on allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }
        candidate++;                          /* move to next integer to test */
    }

    /* nth term is stored at index n-1 */
    return terms[n - 1];
}
