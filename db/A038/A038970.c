/*
 * A038970  Primes p such that 83 is not a square mod p.
 * https://oeis.org/A038970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that Jacobi(83,p) = -1
   (i.e., 83 is not a quadratic residue modulo p).  The first
   term is 2.  The function builds the answer by scanning
   primes and caching the ones that satisfy the Jacobi condition. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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

seq_t a(idx_t n)
{
    /* Cached data over successive calls */
    static uint64_t *pvec    = NULL;   /* primes that satisfy the condition */
    static size_t   pcap     = 0;      /* capacity of pvec */
    static size_t   pcnt     = 0;      /* number stored so far   */
    static uint64_t cur      = 3;      /* odd candidate to test   */
    static bool     initialized = false;

    /* First call: initialise cache with the prime 2 */
    if (!initialized) {
        pcap  = 16;
        pvec  = (uint64_t *)malloc(pcap * sizeof(uint64_t));
        pvec[0] = 2ull;   /* 2 is always the first such prime */
        pcnt   = 1;
        cur    = 3ull;    /* start testing from the next odd number */
        initialized = true;
    }

    /* Grow the array when required */
    if (pcnt >= pcap) {
        pcap *= 2;
        pvec = (uint64_t *)realloc(pvec, pcap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least n stored */
    while (pcnt < (size_t)n) {
        /* test primality of the current candidate */
        if (is_prime_big((seq_t)cur)) {
            /* Jacobi(83, cur) returns -1, 0, or 1.
               We want the -1 case. */
            if (jacobi_symbol(83ULL, cur) == -1) {
                pvec[pcnt++] = cur;
                if (pcnt >= pcap) {               /* double if needed */
                    pcap *= 2;
                    pvec = (uint64_t *)realloc(pvec,
                                                pcap * sizeof(uint64_t));
                }
            }
        }
        cur += 2ull; /* skip even numbers */
    }

    return (seq_t)pvec[n - 1];
}
