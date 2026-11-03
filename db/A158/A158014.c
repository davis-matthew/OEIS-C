/*
 * A158014  Primes p such that (p-1)/8 is also prime.
 * https://oeis.org/A158014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*q + 1 where q is prime and 8*q+1 is also prime (primes
 *            p such that (p-1)/8 is prime)
 * Range:     u8 n <= 5, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Resize a seq_t array to hold at least 'need' elements. */
static void
ensure_cap_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
    while (newcap < need) newcap <<= 1;
    *arr = mem_resize(seq_t, *arr, newcap);
    *cap = newcap;
}

/* Sequence entry a(n). 1‑based indexing. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ---------- Memoisation ---------- */
    static seq_t  *terms  = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    static idx_t next_q = 2;   /* next integer to test for primality */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Find the next prime q. */
        while (!is_prime_big((seq_t)next_q)) ++next_q;
        idx_t q = next_q;
        ++next_q;  /* advance for next search */

        /* Compute p = 8*q + 1 and test primality. */
        seq_t p = ((seq_t)8 * (seq_t)q) + (seq_t)1;
        if (is_prime_big(p)) {
            ensure_cap_seq_t(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = p;
        }
    }

    return terms[n - 1];
}
