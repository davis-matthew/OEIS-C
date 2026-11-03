/*
 * A158192  Prime numbers p such that first digit of p = largest digit of p -
 *          smallest digit of p.
 * https://oeis.org/A158192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 572, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 980 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158192: primes p such that first digit of p = largest digit of p - smallest digit of p
   The function builds the list lazily, caching the qualifying primes. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes */
    static seq_t *qual = NULL;     /* array of qualifying primes   */
    static size_t qual_cap = 0;    /* capacity of qual array       */
    static size_t qual_cnt = 0;    /* number of qualifying primes  */
    static seq_t next = 2;         /* next candidate prime         */

    /* Allocate (or grow) the cache array */
    if (!qual_cap) {
        qual_cap = 32;
        qual = mem_new(seq_t, qual_cap);
        if (!qual) return 0; /* out‑of‑memory fallback */
    }

    /* Generate primes until we have at least n qualifying ones */
    while (qual_cnt < n) {
        if (is_prime_big(next)) {
            /* Compute first digit, max digit, min digit */
            seq_t tmp = next;
            int first = 0;
            /* most significant digit */
            while (tmp >= 10) tmp /= 10;
            first = (int)tmp;

            int dmin = 9, dmax = 0;
            tmp = next;
            /* iterate over all digits */
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (d < dmin) dmin = d;
                if (d > dmax) dmax = d;
                tmp /= 10;
            }

            if (first == dmax - dmin) {
                /* store the qualifying prime */
                if (qual_cnt == qual_cap) {
                    size_t new_cap = qual_cap * 2;
                    seq_t *tmp_ptr = mem_resize(seq_t, qual, new_cap);
                    if (!tmp_ptr) return 0; /* out‑of‑memory fallback */
                    qual = tmp_ptr;
                    qual_cap = new_cap;
                }
                qual[qual_cnt++] = next;
            }
        }
        ++next;
    }

    return qual[n - 1];
}
