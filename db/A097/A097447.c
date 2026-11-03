/*
 * A097447  Primes in the concatenation of consecutive prime-sided isosceles
 *          triangles.
 * https://oeis.org/A097447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A097447: odd prime-sided isosceles triangles
   (x,y,y) where x is prime and y is the next prime after x.
   For each such pair concatenate the three sides to form a
   decimal number and output the one that is prime.
   The sequence is indexed from 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits0.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static inline uint64_t next_prime_after(uint64_t n)
{
    /* Return the first prime greater than n.
       Uses the 64‑bit is_prime routine. */
    uint64_t cand = n + 1;
    if (cand <= 2)
        return 2;
    if ((cand & 1ULL) == 0)
        cand++;               /* make odd */
    for (;;) {
        if (is_prime(cand))
            return cand;
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    /* static state to cache computed terms and prime pairs */
    static seq_t *terms = NULL;     /* cached values */
    static size_t terms_len = 0;    /* number of cached terms */
    static size_t terms_cap = 0;    /* allocated capacity */
    static uint64_t prev_prime = 0;
    static uint64_t next_prime = 0;
    static bool initialized = false;

    /* initialize first prime pair on first call */
    if (!initialized) {
        prev_prime = 2;
        next_prime = next_prime_after(prev_prime); /* 3 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* concatenate prev_prime and next_prime: prev||next */
        seq_t xy = concat_parts(prev_prime, next_prime);

        /* compute factor of 10^{digits(next_prime)} */
        seq_t ten_pow = pow10(num_digits0(next_prime));

        /* final term: (prev||next)*10^{digits(next_prime)} + next_prime
           which is prev||next||next */
        seq_t term = xy * ten_pow + (seq_t)next_prime;

        /* keep it if the concatenation is prime */
        if (is_prime_big(term))
            append_candidate(&terms, &terms_len, &terms_cap, term);

        /* advance to the next consecutive prime pair */
        prev_prime = next_prime;
        next_prime = next_prime_after(next_prime);
    }

    /* return the requested term (1‑based) */
    return terms[n - 1];
}
