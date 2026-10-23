/*
 * A142862  Semiprimes n (A001358) for which A000001(n) is 1.
 * https://oeis.org/A142862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the sequence A142862: Semiprimes pq with p < q and gcd(p, q-1) = 1.
   The implementation builds a sieve of smallest prime factors, scans
   numbers in order, checks that each candidate is a true semiprime  
   (both factors are prime) and that the gcd condition holds.  The
   sequence elements are cached so that subsequent calls are O(1).
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"

static idx_t spf_limit = 0;          /* current sieve limit */
static idx_t *spf = NULL;            /* smallest prime factor table */
static idx_t scan_start = 2;         /* next number to scan */

static seq_t *vals = NULL;           /* array of found sequence values */
static size_t val_cap = 0;           /* capacity of vals */
static size_t val_len = 0;           /* number of values stored */

/* Ensure that vals contains at least n elements */
static void ensure_sequence(idx_t n)
{
    while (val_len < n)
    {
        /* Extend sieve if needed */
        if (scan_start >= spf_limit)
        {
            idx_t new_limit = spf_limit ? spf_limit * 2 : 1048576ULL; /* 1MiB start */
            if (new_limit < scan_start + 1)
                new_limit = scan_start + 1;

            free(spf);
            build_spf(new_limit, &spf);
            spf_limit = new_limit;
        }

        /* Scan newly available numbers */
        for (; scan_start < spf_limit && val_len < n; ++scan_start)
        {
            if (scan_start < 6) /* smallest semiprime is 6 */
                continue;

            idx_t p = spf[scan_start];
            if (p == 0 || p == scan_start) /* skip primes or uninitialized */
                continue;

            idx_t q = scan_start / p;
            if (q == p) /* skip squares */
                continue;

            /* Ensure q is prime */
            if (!is_prime(q))
                continue;

            seq_t g = gcd((seq_t)p, (seq_t)(q - 1));
            if (g == 1)
            {
                if (val_len == val_cap)
                {
                    val_cap = val_cap ? val_cap * 2 : 16;
                    vals = (seq_t *)realloc(vals, val_cap * sizeof(seq_t));
                    if (!vals) abort(); /* sanity check */
                }
                vals[val_len++] = (seq_t)scan_start;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_sequence(n);
    return vals[n - 1];
}
