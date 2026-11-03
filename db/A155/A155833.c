/*
 * A155833  Primes in which smallest digit is final digit.
 * https://oeis.org/A155833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155833: primes whose smallest digit is the last digit.
   Computed by generating primes in increasing order and collecting
   those satisfying the digit condition. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *qualprimes = NULL;   /* array of qualifying primes */
static size_t qualcnt   = 0;       /* number of qualifying primes stored */
static size_t qualcap   = 0;       /* current capacity of qualprimes */
static seq_t next_candidate = 2;   /* next number to test for primality */
static bool initialized = false;  /* flag for one‑time initialization */

/* check if a prime satisfies the OEIS condition */
static bool satisfies_condition(seq_t p) {
    seq_t tmp = p;
    unsigned min_digit = 9;
    unsigned last_digit = 0;
    bool first = true;

    while (tmp > (seq_t)0) {
        unsigned d = (unsigned)(tmp % (seq_t)10);
        if (d < min_digit) min_digit = d;
        if (first) {
            last_digit = d;
            first = false;
        }
        tmp /= (seq_t)10;
    }
    return last_digit == min_digit;
}

/* enlarge the array of qualifying primes if needed */
static void ensure_capacity(void) {
    if (qualcnt >= qualcap) {
        size_t newcap = qualcap ? qualcap << 1 : 16;
        seq_t *newarr = seq_alloc_array(newcap);
        for (size_t i = 0; i < qualcnt; ++i) {
            newarr[i] = qualprimes[i];
        }
        seq_free_array(qualprimes);
        qualprimes = newarr;
        qualcap    = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    if (!initialized) {
        initialized = true;        /* next_candidate already 2 */
    }

    while (qualcnt < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            if (satisfies_condition(next_candidate)) {
                ensure_capacity();
                qualprimes[qualcnt++] = next_candidate;
            }
        }
        /* advance to next candidate: 2 → 3, then odd numbers only */
        if (next_candidate == (seq_t)2) {
            next_candidate = (seq_t)3;
        } else {
            next_candidate += (seq_t)2;
        }
    }

    return qualprimes[(size_t)n - 1];
}
