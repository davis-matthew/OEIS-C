/*
 * A163319  Primes of the form 3^n-n^3.
 * https://oeis.org/A163319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163319 – Primes of the form 3^k – k^3 (k = 1, 2, …)
 * This function returns the n‑th such prime, where n ≥ 1.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache of found primes, growing as needed */
    static seq_t *primes = NULL;
    static size_t cap = 0, sz = 0;
    static idx_t exp = 0;            /* last exponent processed */

    /* generate primes until we have at least n of them */
    while (sz < n) {
        exp++;                      /* next exponent candidate */

        seq_t power = ipow((seq_t)3, (seq_t)exp);
        seq_t cube  = (seq_t)exp * (seq_t)exp * (seq_t)exp;
        seq_t val   = power - cube;

        if (is_prime_big(val)) {
            /* ensure capacity and append the prime */
            grow_seq(&primes, &cap, sz + 1);
            primes[sz++] = val;
        }
    }

    return primes[(size_t)(n - 1)];
}
