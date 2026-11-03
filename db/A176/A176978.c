/*
 * A176978  Numbers n such that n^2 + 13 and n^2 + 13^2 are primes.
 * https://oeis.org/A176978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 724, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n^2 + 13 and n^2 + 13^2 are primes.
   The function returns the nth such integer (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

/* Cache of found terms.  */
static seq_t *terms = NULL;
static idx_t count = 0;          /* number of cached terms */
static idx_t cap   = 0;          /* allocated capacity of 'terms' */
static idx_t last_candidate = 0; /* last candidate n tested */

/* Return true if x is prime, choosing the fastest routine available. */
static inline bool is_prime_check(seq_t x)
{
    if (sizeof(seq_t) <= 8) {
        return is_prime_seq((uint64_t)x);
    } else {
        return is_prime_big(x);
    }
}

/* Ensure that 'terms' can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (cap >= needed)
        return;
    idx_t newcap = cap ? cap << 1 : 4;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, terms, newcap);
    if (!tmp) {
        abort(); /* in production, report error */
    }
    terms = tmp;
    cap   = newcap;
}

/* Return the n‑th term of A176978 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* sequence defined for n ≥ 1; 0 is a dummy */

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        last_candidate++;                     /* next integer to test */
        seq_t c  = (seq_t)last_candidate;     /* candidate n      */
        seq_t c2 = c * c;                     /* n^2              */

        seq_t p1 = c2 + (seq_t)13;            /* n^2 + 13        */
        seq_t p2 = c2 + (seq_t)169;           /* n^2 + 13^2      */

        if (is_prime_check(p1) && is_prime_check(p2)) {
            ensure_capacity(count + 1);
            terms[count++] = c;
        }
    }

    return terms[n - 1];  /* 1‑based indexing */
}
