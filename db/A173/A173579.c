/*
 * A173579  Natural numbers n which give primes when 1331 = 11^3 is prefixed.
 * https://oeis.org/A173579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 913 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173579: a(n) = the n‑th natural number m (m ≥ 1) for which the decimal concatenation 1331‖m is prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Cache of found terms.  The cache is static so that calls may arrive in any order. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_size = 0;
    static idx_t tested = 0;              /* Largest candidate examined so far */

    /* Generate terms until we have at least n of them. */
    while (terms_size < n)
    {
        idx_t cand = tested + 1;
        tested = cand;

        /* If cand ≡ 1 (mod 3) the concatenated number is a multiple of 3, skip it. */
        if (cand % 3 == 1) continue;

        /* Concatenate 1331 and cand. */
        seq_t val = concat_parts(1331, cand);

        /* If the concatenated number is not prime, skip this candidate. */
        if (!is_prime_big(val)) continue;

        /* Grow the cache if necessary. */
        if (terms_size + 1 > terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 8;
            seq_t *tmpv = mem_resize(seq_t, terms, new_cap);
            if (!tmpv) abort();    /* allocation failed */
            terms = tmpv;
            terms_cap = new_cap;
        }

        terms[terms_size++] = (seq_t)cand;
    }

    return terms[n - 1];
}
