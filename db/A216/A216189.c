/*
 * A216189  Numbers n whose odd Collatz steps (except for 1) are all primes.
 * https://oeis.org/A216189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_pow2, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Numbers n>0 whose odd Collatz steps (excluding n itself
 * and the final 1) are all primes.  Powers of two are excluded.  The
 * function searches for terms incrementally and caches them for reuse. */

#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/is_pow2.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* ------------------------------------------------------------------ */
/* Test whether a candidate n satisfies the definition.  The function
 * works with any seq_t that supports the operators used below.  It
 * distinguishes between odd and even starting values: if n itself is
 * odd, the first odd number encountered (which is n) is not checked
 * for primality; all subsequent odd numbers (except 1) must be prime. */
static bool is_good(seq_t n)
{
    /* Powers of two are excluded. */
    if (is_pow2(n))
        return false;

    bool skip_first = (n & 1) != 0;   /* skip check if n is odd */
    bool first_odd_done = false;      /* flag for the first odd seen */

    seq_t x = n;
    while (x > (seq_t)1) {
        if ((x & 1) != 0) { /* odd step */
            if (!(skip_first && !first_odd_done)) {
                if (!is_prime_big(x))
                    return false;
            }
            first_odd_done = true;
            x = 3 * x + 1;
        } else {            /* even step: strip all factors of two */
            while ((x & 1) == 0)
                x = x >> 1;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of computed terms. */
static seq_t   *terms   = NULL;  /* dynamic array of seq_t       */
static size_t  count   = 0;      /* number of stored terms       */
static size_t  capacity = 0;     /* size of allocated buffer     */
static idx_t   checked  = 0;     /* last integer inspected       */

/* Grow to at least new_cap and copy old data. */
static void grow_cache(size_t new_cap)
{
    seq_t *new_terms = seq_alloc_new_array(new_cap);
    if (terms) {
        for (size_t i = 0; i < count; ++i)
            new_terms[i] = terms[i];
        seq_free_array(terms);
    }
    terms = new_terms;
    capacity = new_cap;
}

/* Append a new term to the cache, growing the buffer when needed. */
static void add_term(seq_t v)
{
    if (count == capacity)
        grow_cache(capacity ? capacity << 1 : 16);

    terms[count++] = v;
}

/* Ensure that the first n terms have been generated. */
static void ensure_terms(idx_t n)
{
    if (n <= count)
        return;

    if (checked < 3)
        checked = 3;               /* first admissible candidate */

    while (count < n) {
        seq_t cand = (seq_t)checked++;
        if (is_good(cand))
            add_term(cand);
    }
}

/* ------------------------------------------------------------------ */
/* Main entry point for the sequence A216189. */
seq_t a(idx_t n)
{
    /* a(1) is the first term (3). */
    ensure_terms(n);
    return terms[n - 1];
}
