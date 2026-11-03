/*
 * A054211  Numbers k such that k concatenated with k-1 is prime.
 * https://oeis.org/A054211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 3299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9591 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054211: Numbers k such that k concatenated with k-1 is prime.
   Computed by incremental search. The n-th term a(n) is returned. */
/* Dependencies: is_prime_u64 from <../utils/is_prime_u64.h>
                  is_prime_big from <../utils/is_prime_big.h> */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* Helper: compute 10^(number of decimal digits of x), where x ≥ 0. */
static seq_t pow10_digits(uint64_t x)
{
    seq_t res = 1;
    do {
        res *= 10;
        x /= 10;
    } while (x > 0);
    return res;
}

static seq_t *terms = NULL;          /* dynamic array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of the array */
static uint64_t next_k = 4;          /* next candidate k to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, but safe */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t pow10 = pow10_digits(next_k - 1);
        seq_t concat = k * pow10 + (seq_t)(next_k - 1);

        bool prime;
        if (sizeof(seq_t) <= 8) {
            /* seq_t fits in 64 bits – use the fast routine. */
            prime = is_prime_u64((uint64_t)concat);
        } else {
            prime = is_prime_big(concat);
        }

        if (prime) {
            /* Grow storage if necessary */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();    /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
