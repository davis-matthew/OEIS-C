/*
 * A124411  Numbers k such that 2k+1, 4k+1, 6k+1, 8k+1, 10k+1 and 12k+1 are
 *          primes.
 * https://oeis.org/A124411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   is_prime_big, modinv_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A124411: the integer k such that 2k+1,4k+1,6k+1,
   8k+1,10k+1 and 12k+1 are all prime.  The function searches
   successive values of k by first filtering k with small prime
   sieves, then testing all six linear forms with a full
   primality test.  The results are cached so calls arrive in any
   order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/modinv_mod.h"

#define NUM_SMALL 8

/* Coefficients of the linear forms in the form a * m + 1,
   where k = 105 * m.  Thus 2k+1 = 210*m+1, etc. */
static const int COEFFS[6] = {210, 420, 630, 840, 1050, 1260};

/* Small primes used for pre‑sieving.  2,3,5,7 are omitted because
   the 105‑multiple requirement already removes them. */
static const int SMALL_PRIMES[NUM_SMALL] = {11, 13, 17, 19, 23, 29, 31, 37};

/* For each small prime p, allowed[p] stores a byte per residue
   r (0 ≤ r < p) that is true if no a*m+1 is ≡0 (mod p) when
   m ≡ r (mod p).  The array of pointers is initialized once. */
static unsigned char *allowed[NUM_SMALL] = {NULL};
static bool filter_initialized = false;

/* Initialise the residue tables once. */
static void init_filter_tables(void)
{
    for (int i = 0; i < NUM_SMALL; ++i) {
        int p = SMALL_PRIMES[i];
        allowed[i] = (unsigned char *)malloc((size_t)p);
        for (int r = 0; r < p; ++r) allowed[i][r] = 1;  /* all allowed initial */

        for (int j = 0; j < 6; ++j) {
            int a = COEFFS[j];
            int a_mod = a % p;
            if (a_mod == 0) continue;               /* a*m+1 ≡ 1 (mod p) → no restriction */
            int inv = modinv_mod(a_mod, p);
            int forb = (p - inv) % p;                /* -inv mod p */
            allowed[i][forb] = 0;                    /* forbid this residue */
        }
    }
    filter_initialized = true;
}

/* Memoised cache of found values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t current_m = 0;     /* m = k / 105 for the current candidate */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 – return 0 */

    if (!filter_initialized) init_filter_tables();

    /* Grow the cache if necessary. */
    if (cache_len < n) {
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
    }

    /* Find additional terms until we have at least n terms. */
    while (cache_len < n) {
        /* Small–prime sieve test. */
        bool ok = true;
        for (int i = 0; i < NUM_SMALL; ++i) {
            int p = SMALL_PRIMES[i];
            if (!allowed[i][(int)(current_m % (idx_t)p)]) { /* pointer indexing requires int */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Compute k from m. */
            seq_t k = (seq_t)105 * (seq_t)current_m;

            /* Test all six linear forms for primality. */
            bool all_prime = true;
            for (int j = 0; j < 6; ++j) {
                seq_t candidate = (seq_t)COEFFS[j] * (seq_t)current_m + 1;
                if (!is_prime_big(candidate)) {
                    all_prime = false;
                    break;
                }
            }

            if (all_prime) {
                /* Store the found value. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
        ++current_m;
    }

    return cache[n - 1];
}
