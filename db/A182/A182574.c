/*
 * A182574  Primes that have seven terms in their Zeckendorf representation.
 * https://oeis.org/A182574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence consists of the primes p such that the number of
 *            terms in the Zeckendorf representation of p equals 7. The
 *            function computes the n‑th such prime
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple sieve-like search combined with a greedy
   Zeckendorf decomposition.  All data are cached statically so
   that calls to a(idx_t) in arbitrary order return the correct
   result within the time limits. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Grow an array of seq_t to hold at least needed items.  The array is
   resized with seq_realloc, which copies over existing elements and
   handles class types correctly. */
static void grow_array(seq_t **arr, size_t *cap, size_t needed) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    *arr = seq_realloc(*arr, *cap, new_cap);
    *cap = new_cap;
}

/* Count the number of terms in the Zeckendorf representation of the
   (prime) value p.  The Fibonacci table is built on demand and
   stored in the static array fibs. */
static size_t zeckendorf_terms(seq_t p,
                               seq_t **fibs, size_t *fcount,
                               size_t *fib_cap, seq_t *fmax) {
    /* Build the Fibonacci table up to the first Fibonacci > p. */
    if (p > *fmax) {
        /* Initialise the table if it is empty. */
        if (*fcount == 0) {
            grow_array(fibs, fib_cap, 16);
            (*fibs)[0] = (seq_t)1;   /* F1 = 1 */
            (*fibs)[1] = (seq_t)2;   /* F2 = 2 */
            *fcount = 2;
        }

        /* Append further Fibonacci numbers until they exceed p. */
        while (p > (*fibs)[*fcount - 1]) {
            if (*fcount >= *fib_cap)
                grow_array(fibs, fib_cap, (*fib_cap) * 2 + 16);
            (*fibs)[*fcount] = (*fibs)[*fcount - 2] + (*fibs)[*fcount - 1];
            ++(*fcount);
        }
        /* Store a number greater than the last used Fibonacci for
           the next call. */
        *fmax = (*fibs)[*fcount - 1] > p ? (*fibs)[*fcount - 1] : p + 1;
    }

    /* Greedy decomposition using the now complete table. */
    size_t count = 0;
    seq_t rem = p;
    for (size_t i = *fcount; i > 0; --i) {
        if ((*fibs)[i - 1] <= rem) {
            rem -= (*fibs)[i - 1];
            ++count;
            if (!rem) break;
        }
    }
    return count;
}

/* Main OEIS function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* n < 1 is undefined for the OEIS offset */

    /* Static cache for Fibonacci numbers needed for Zeckendorf
       calculations. */
    static seq_t *fibs = NULL;
    static size_t fib_count = 0;
    static size_t fib_cap   = 0;
    static seq_t fib_max    = 0;

    /* Static cache for the primes with exactly seven Zeckendorf terms. */
    static seq_t *primes7 = NULL;
    static size_t primes7_count = 0;
    static size_t primes7_cap   = 0;

    size_t target = (size_t)(n - 1);   /* zero‑based index */

    /* Ensure the primes cache is large enough for the request. */
    grow_array(&primes7, &primes7_cap, target + 1);

    /* If we already have the needed prime, return it immediately. */
    if (primes7_count > target)
        return primes7[target];

    /* Begin searching from the next integer after the last cached prime. */
    uint64_t candidate = (primes7_count == 0) ? 2ULL
                                              : (uint64_t)primes7[primes7_count - 1] + 1ULL;
    /* Search until we have enough primes. */
    while (primes7_count <= target) {
        /* Skip even numbers greater than 2. */
        if (candidate > 2 && (candidate & 1ULL) == 0ULL) {
            ++candidate;
            continue;
        }

        /* Test primality on arbitrary‑precision numbers. */
        seq_t cand_seq = (seq_t)candidate;
        if (is_prime_big(cand_seq)) {
            /* Count Zeckendorf terms. */
            size_t terms = zeckendorf_terms(cand_seq,
                                            &fibs, &fib_count,
                                            &fib_cap, &fib_max);
            if (terms == 7) {
                /* Store the prime. */
                primes7[primes7_count++] = cand_seq;
            }
        }
        ++candidate;
        /* Candidate will never wrap for n ≤ 10000. */
    }

    return primes7[target];
}
