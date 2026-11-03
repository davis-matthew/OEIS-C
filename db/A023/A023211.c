/*
 * A023211  Primes p such that 3*p + 10 is also prime.
 * https://oeis.org/A023211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 868, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 869 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 3p+10 is also prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
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

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Static storage for memoised values. */
    static seq_t *list     = NULL;  /* array storing the qualifying primes */
    static size_t sz       = 0;     /* number of primes stored so far  */
    static size_t cap      = 0;     /* current allocation capacity         */
    static uint64_t last   = 2;     /* last tested integer (even initially) */

    /* First invocation: initialise dynamic array. */
    if (list == NULL) {
        cap = 64;
        list = mem_new(seq_t, cap);
        /* begin search just before the first odd prime 3 */
        last = 2;
    }

    /* Generate terms until the desired index is reached. */
    while (sz < n) {
        /* Advance to the next odd candidate. */
        if (last < 3)
            last = 3;
        else
            last += 2;

        /* Test primality of the candidate. */
        if (!is_prime(last))
            continue;

        /* Compute 3*last + 10 without overflow. */
        uint64_t tmp = 3 * last + 10ULL;
        seq_t q = (seq_t)tmp;

        /* Test primality of 3*last+10. */
        if (!is_prime_big(q))
            continue;

        /* Store the qualifying prime; grow storage if necessary. */
        if (sz == cap) {
            cap *= 2;
            list = mem_resize(seq_t, list, cap);
        }
        list[sz++] = (seq_t)last;
    }

    /* Return the asked term (1‑based index). */
    return list[n - 1];
}
