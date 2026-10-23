/*
 * A142140  Primes congruent to 31 mod 37.
 * https://oeis.org/A142140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime number congruent to 31 (mod 37)
 * Range:     u8 n <= 2, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Integer square root for type idx_t using binary search */
static inline idx_t integer_sqrt(idx_t n) {
    idx_t high = 1;
    /* Find high such that high*high > n */
    while (high * high <= n) {
        high <<= 1;
        /* Prevent overflow */
        if (high == 0) {
            high = ((idx_t)1 << ((sizeof(idx_t)*8 - 1) / 2));
            break;
        }
    }
    idx_t low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

/* Static tables for prime generation */
static idx_t   *prime_list   = NULL;  /* all primes discovered */
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;
static idx_t   *residue_list = NULL;  /* primes == 31 (mod 37) */
static size_t   residue_cnt  = 0;
static size_t   residue_cap  = 0;
static idx_t   last_checked  = 0;

/* Ensure we have at least n_needed residues in the cache */
static void
ensure_residue_cache(idx_t n_needed)
{
    if (residue_cnt >= (size_t)n_needed)
        return;

    /* Initial allocation */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        residue_cap = 16;
        residue_list = (idx_t *)malloc(residue_cap * sizeof(idx_t));
    }

    /* Start scanning from the next number after the last checked value */
    idx_t candidate = (last_checked == 0) ? 2 : last_checked + 1;

    while (residue_cnt < (size_t)n_needed) {
        /* Skip even numbers > 2 */
        if (candidate > 2 && (candidate & 1) == 0)
            candidate++;

        /* Check primality of candidate */
        int is_prime = 1;
        idx_t sqrt_c = integer_sqrt(candidate);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_c; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Add to general prime list */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_cnt++] = candidate;

            /* If it matches the residue condition, store it */
            if (candidate % 37u == 31u) {
                if (residue_cnt == residue_cap) {
                    residue_cap *= 2;
                    residue_list = (idx_t *)realloc(residue_list,
                                                    residue_cap * sizeof(idx_t));
                }
                residue_list[residue_cnt++] = candidate;
            }
        }
        last_checked = candidate;
        candidate++;   /* move to next integer */
    }
}

/* Main entry point for the sequence */
seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;  /* Undefined for indices below offset */
    }

    ensure_residue_cache(n);
    return (seq_t)residue_list[(size_t)n - 1];
}
