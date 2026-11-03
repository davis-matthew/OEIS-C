/*
 * A076805  Triskaidekaphobic or 13-free primes: primes that do not contain
 *          the number 13.
 * https://oeis.org/A076805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that does not contain the decimal
 *            substring "13"
 * Range:     u8 n <= 49, u16 n <= 6142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9516 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates primes on demand, skipping any prime whose decimal
 * representation contains "13".  The results are memoized so that subsequent
 * calls for arbitrary n work in O(1) after the initial search. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Check whether a 64‑bit integer contains the substring 13. */
static bool contains13_uint64(uint64_t x) {
    while (x >= 10) {
        if ((x % 100) == 13) return true;  /* two least significant digits are 13 */
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;    /* memoized list of found terms */
    static size_t term_count = 0;  /* number of stored terms */
    static size_t term_cap   = 0;  /* current allocation capacity */
    static uint64_t next_candidate = 2;   /* next integer to test */

    /* Expand storage as needed. */
    if (term_count >= n) return terms[n - 1];

    while (term_count < n) {
        /* Find the next prime that does not contain 13. */
        for (;;) {
            seq_t cand = (seq_t)next_candidate;

            if (is_prime_big(cand) && !contains13_uint64(next_candidate)) {
                /* Make room for the new term. */
                if (term_count >= term_cap) {
                    term_cap = term_cap == 0 ? 16 : term_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, terms, term_cap);
                    if (!tmp) exit(1);
                    terms = tmp;
                }
                terms[term_count++] = cand;
                ++next_candidate;      /* prepare for next search */
                break;
            }
            ++next_candidate;
        }
    }
    return terms[n - 1];
}
