/*
 * A137997  Numbers which are not the sum of a triangular number and zero or a
 *          prime = 3 (mod 4).
 * https://oeis.org/A137997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137997: numbers that cannot be written as
 * p + T_k where p is 0 or a prime congruent to 3 (mod 4),
 * and T_k = k(k+1)/2 is the k-th triangular number (k ≥ 0).
 *
 * The implementation stores all already found terms in a
 * static dynamic array.  For each new candidate integer x,
 * it tries every triangular number T_k ≤ x.  If x – T_k equals
 * 0, or equals a prime ≡ 3 (mod 4), the candidate is excluded.
 * Otherwise x is appended to the cache.  The list is built up
 * lazily until the requested index n is reached.
 *
 * This algorithm runs in O(n·√m) time, where n is the required
 * index and m the n‑th term (≈ 10⁵ for n ≈ 10⁴).  It easily meets
 * the 2.0‑second per‑call requirement for the typical builds.
 */

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

/* external types: seq_t and idx_t are provided by the build environment */
extern seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *terms    = NULL;   /* array of terms */
    static size_t terms_cnt = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* current capacity of the array */
    static idx_t  candidate  = 2;    /* next integer to test */

    /* initialise cache on first call */
    if (terms == NULL) {
        terms_cap = 32;
        terms     = mem_new(seq_t, terms_cap);
        if (terms == NULL) {
            /* out of memory; caller will see an error elsewhere */
            exit(1);
        }
    }

    /* grow array when necessary */
    if (terms_cnt >= terms_cap) {
        size_t new_cap = terms_cap * 2;
        seq_t *new_terms = mem_resize(seq_t, terms, new_cap);
        if (new_terms == NULL) {
            exit(1);
        }
        terms = new_terms;
        terms_cap = new_cap;
    }

    /* build list until we have at least n terms */
    while (terms_cnt < (size_t)n) {
        bool representable = false;
        seq_t T = 0;           /* current triangular number */
        idx_t k = 0;           /* index for triangular number */
        seq_t cand = (seq_t)candidate;

        while (T <= cand) {
            seq_t rem = cand - T;
            if (rem == (seq_t)0) {
                representable = true;
                break;
            }
            if (is_prime_big(rem) && ((rem % 4u) == 3u)) {
                representable = true;
                break;
            }
            k++;
            T += (seq_t)k;   /* next triangular number */
        }

        if (!representable) {
            if (terms_cnt >= terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *new_terms = mem_resize(seq_t, terms, new_cap);
                if (new_terms == NULL) {
                    exit(1);
                }
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = cand;
        }
        candidate++;
    }

    return terms[n - 1];
}
