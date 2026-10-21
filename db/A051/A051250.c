/*
 * A051250  Numbers whose reduced residue system consists of 1 and prime
 *          powers only.
 * https://oeis.org/A051250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A051250: Numbers whose reduced residue system consists of
   1 and prime powers only.  The sequence is finite; the largest term
   is 60.  This implementation builds the list once by testing integers
   up to a safe upper bound and then returns the requested term. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* --- Helper: test whether x (>1) is a prime power (p^k, k>=1) */
static bool is_prime_power(seq_t x)
{
    if (x < 2) return false;
    /* Find the smallest prime divisor by trial division. */
    idx_t p = 2;
    while ((seq_t)p * (seq_t)p <= x) {
        if (x % (seq_t)p == 0) break;
        ++p;
    }
    /* If no divisor found, x is prime. */
    if ((seq_t)p * (seq_t)p > x) return true;
    while (x % (seq_t)p == 0) x /= (seq_t)p;
    return x == 1;
}

/* --- Helper: test whether n satisfies the definition of the
   sequence (all elements of the reduced residue system are 1 or prime powers). */
static bool is_valid(seq_t n)
{
    idx_t n_idx = (idx_t)n;
    for (idx_t g = 2; g < n_idx; ++g) {
        if (gcd((seq_t)g, n) == 1) {
            if (!is_prime_power((seq_t)g))
                return false;
        }
    }
    return true;
}

/* ---------- Static cache of found terms ---------- */
static seq_t *valid = NULL;
static size_t valid_count = 0;
static size_t valid_cap = 0;

/* Initialize the cache by searching up to an upper bound (sufficiently
   larger than the last known term).  The bound may be increased if
   new terms are discovered in the future. */
static void init_valid(void)
{
    if (valid != NULL) return;            /* already initialized */

    /* Start with a small capacity and grow as needed. */
    valid_cap = 32;
    valid = (seq_t *)malloc(valid_cap * sizeof(seq_t));
    if (!valid) return;                   /* allocation failure: silently fall back to zero results */

    const uint64_t UPPER_BOUND = 100;  /* safe bound: 60 is the largest known term */
    for (uint64_t cand = 1; cand <= UPPER_BOUND; ++cand) {
        if (is_valid((seq_t)cand)) {
            if (valid_count == valid_cap) {
                size_t new_cap = valid_cap * 2;
                seq_t *tmp = (seq_t *)realloc(valid, new_cap * sizeof(seq_t));
                if (!tmp) { free(valid); valid = NULL; return; }
                valid = tmp;
                valid_cap = new_cap;
            }
            valid[valid_count++] = (seq_t)cand;
        }
    }
}

/* --- Public function required by the build --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but n==0 is undefined */
    init_valid();
    if (!valid) return 0;          /* allocation failure */
    if ((idx_t)n <= (idx_t)valid_count)
        return valid[(size_t)n - 1];
    return 0;                      /* n beyond known terms */
}
