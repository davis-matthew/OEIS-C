/*
 * A032727  Numbers k such that k prefixed by '2' and followed by '9' is
 *          prime.
 * https://oeis.org/A032727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the integer formed by prefixing
 *            '2' and suffixing '9' to k is prime. That integer is
 *            2*10^{digits(k)+1} + k*10 + 9
 * Range:     u8 n <= 2, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 870 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

static seq_t *cache = NULL;      /* array of found k values */
static size_t cap = 0;           /* current capacity of cache */
static size_t cnt = 0;           /* number of stored terms */
static idx_t last_k = 0;         /* last k tested */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Return a(n) for n >= 1.  No defined behavior for n == 0. */
    if (n == 0) return 0;

    /* Allocate initial cache space if needed */
    if (cache == NULL) {
        cap = 128;
        cache = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them */
    while (cnt < n) {
        ++last_k;                                   /* next k to test */

        /* Compute e = digits(k) + 1 */
        idx_t e = (idx_t)num_digits_idx(last_k) + 1;

        /* Form the candidate number: 2*10^e + k*10 + 9 */
        seq_t candidate = 2 * pow10(e) + ((seq_t)last_k * 10) + 9;

        /* Check primality */
        if (is_prime_big(candidate)) {
            /* Grow cache if necessary */
            if (cnt >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[cnt++] = last_k;
        }
    }

    /* Return the cached value */
    return cache[n - 1];
}
