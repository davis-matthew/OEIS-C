/*
 * A071680  Primes that are the arithmetic mean of their prime predecessor and
 *          another prime.
 * https://oeis.org/A071680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) such that 2*prime(n)-prime(n-1) is prime. The
 *            sequence is defined for n ≥ 1, with a(1)=5
 * Range:     u8 n <= 7, u16 n <= 580, u32 n <= 11000, u64 n <= 11000,
 *            u128 n <= 11000, big any n
 * Validated: standard, pass, 63 terms through n = 8941 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities (provided by the build) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"
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

/* Standard headers */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Static state ---------- */
static seq_t *terms   = NULL; /* collected sequence values          */
static size_t term_sz = 0;    /* number of stored terms             */
static size_t cap     = 0;    /* current allocation size            */
static uint64_t prev_prime = 2; /* previous prime in the full sequence */

/* Append a new value to the sequence array */
static void append_term(seq_t val) {
    if (term_sz == cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *new_arr = mem_resize(seq_t, terms, new_cap);
        if (!new_arr) abort();              /* out of memory */
        terms = new_arr;
        cap   = new_cap;
    }
    terms[term_sz++] = val;
}

/* Generate terms until we have at least n of them */
static void generate_terms(idx_t n) {
    while (term_sz < n) {
        uint64_t cur = next_prime(prev_prime);          /* next prime after prev */
        seq_t test = 2 * (seq_t)cur - (seq_t)prev_prime;
        if (is_prime_big(test)) append_term((seq_t)cur);
        prev_prime = cur;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, but harmless */
    generate_terms(n);              /* ensure term n is available */
    return terms[n - 1];            /* 1‑based OEIS indexing */
}
