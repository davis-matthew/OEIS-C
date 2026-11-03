/*
 * A158281  Prime numbers p such that prime = abs(smallest digit of p - sum of
 *          all the other digits of p).
 * https://oeis.org/A158281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 947 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property based search: primes p such that abs(smallest digit of p -
   sum of the other digits of p) is itself prime. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility helpers */
#include <stdint.h>
#include "../utils/get_digits.h"
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

/* Returns true if prime p satisfies the defining property. */
static bool has_property(seq_t p)
{
    /* Digits are stored in little‐endian order: least significant digit first. */
    uint8_t buf[128];
    size_t len;
    get_digits(p, buf, &len);
    if (len == 0)
        return false;                /* empty number – should never happen */

    int min_d = 10;                  /* larger than any decimal digit */
    int sum   = 0;
    for (size_t i = 0; i < len; ++i)
    {
        int d = buf[i];
        sum += d;
        if (d < min_d)
            min_d = d;
    }

    int sum_other = sum - min_d;
    int diff      = (min_d > sum_other) ? (min_d - sum_other) : (sum_other - min_d);

    /* diff is small (≤ 9 * number of digits), test it for primality. */
    return is_prime_big((seq_t)diff);
}

/* Returns the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1; behaviour for n<1 is irrelevant */

    /* Static storage for memoisation. */
    static seq_t *terms      = NULL; /* found primes satisfying the property   */
    static size_t term_count = 0;    /* current number of stored terms          */
    static size_t term_cap   = 0;    /* current capacity of the array           */
    static seq_t next_candidate = 2; /* next integer to test (prime search)      */

    /* Ensure capacity large enough for at least n terms. */
    if (term_cap < n)
    {
        size_t new_cap = term_cap ? (term_cap << 1) : 64;
        if (new_cap < n)
            new_cap = n;
        seq_t *new_arr = mem_resize(seq_t, terms, new_cap);
        if (new_arr == NULL)
            return 0;               /* allocation failure – return zero */
        terms = new_arr;
        term_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (term_count < n)
    {
        /* Find the next prime ≥ next_candidate. */
        seq_t p = next_candidate;
        while (!is_prime_big(p))
            ++p;

        /* If it satisfies the property, store it. */
        if (has_property(p))
            terms[term_count++] = p;

        /* Prepare the next candidate for the following iteration. */
        next_candidate = p + 1;
    }

    return terms[n - 1];
}
