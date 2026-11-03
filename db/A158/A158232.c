/*
 * A158232  Numbers which yield primes when "13" is prefixed or appended: N
 *          natural number is a member of the sequence, if P="13N" (prefixed
 *          13) and A="N13" (appended 13) are prime.
 * https://oeis.org/A158232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 955 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A158232: numbers N such that 13*N and N*100+13 are prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the n-th term (n ≥ 1) of OEIS A158232 */
seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* how many terms stored       */
    static size_t terms_cap = 0;   /* allocated capacity           */
    static idx_t nextN = 1;        /* next natural number to test */

    while (terms_len < n) {
        /* number of decimal digits of nextN */
        int d = num_digits_idx((idx_t)nextN);
        seq_t pow10d = pow10((idx_t)d);                 /* 10^d */

        /* 13 * 10^d + nextN  → prefixed number */
        seq_t pref = ((seq_t)13) * pow10d + (seq_t)nextN;

        /* nextN * 100 + 13   → suffixed number */
        seq_t suff = (seq_t)nextN * ((seq_t)100) + ((seq_t)13);

        if (is_prime_big(pref) && is_prime_big(suff)) {
            /* grow cache array if needed */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) return (seq_t)0;  /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)nextN;
        }
        nextN++;
    }

    return terms[n - 1];
}
