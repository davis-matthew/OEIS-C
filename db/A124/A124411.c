/*
 * A124411  Numbers k such that 2k+1, 4k+1, 6k+1, 8k+1, 10k+1 and 12k+1 are
 *          primes.
 * https://oeis.org/A124411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 100 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include <stdint.h>
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

/* Compute modular inverse of a mod p (p is prime, 1 < a < p). */
static int modinv_mod(int a, int p)
{
    a %= p;
    if (a < 0) a += p;
    for (int x = 1; x < p; ++x) {
        if ((a * x) % p == 1) return x;
    }
    return -1;          /* never happens for prime p */
}

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
            cache = mem_new(seq_t, cache_cap);
        } else if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = mem_resize(seq_t, cache, new_cap);
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
                    cache = mem_resize(seq_t, cache, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
        ++current_m;
    }

    return cache[n - 1];
}
