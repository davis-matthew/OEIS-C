/*
 * A127178  Primes whose "Look And Say" descriptions from right to left (in
 *          the sense of method B, i.e., digit-indication followed by
 *          frequency) are also primes.
 * https://oeis.org/A127178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime p such that the right‑to‑left
   "Look‑and‑Say" description of p (digit followed by run length)
   is also prime.  The function generates terms by searching primes
   and memoising them. */

#include <stdlib.h>
#include <stdbool.h>
/* Helper functions available from utils */
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Build the "Look‑and‑Say" description from right to left. */
static seq_t descr_from_prime(idx_t p)
{
    seq_t descr = 0;
    idx_t tmp = p;
    int digits[64];
    int len = 0;

    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    if (len == 0) return 0; /* should never happen for primes */

    for (int i = 0; i < len; ) {
        int d = digits[i];
        int freq = 1;
        ++i;
        while (i < len && digits[i] == d) { ++freq; ++i; }

        /* append digit d */
        descr = descr * 10 + d;

        /* append frequency in decimal */
        int fd[20];
        int fdlen = 0;
        do {
            fd[fdlen++] = freq % 10;
            freq /= 10;
        } while (freq > 0);
        for (int k = fdlen - 1; k >= 0; --k)
            descr = descr * 10 + fd[k];
    }
    return descr;
}

static seq_t *seq_cache = NULL;   /* memoised sequence values   */
static size_t seq_cap  = 0;       /* allocated capacity         */
static size_t seq_len  = 0;       /* number of terms computed   */
static idx_t  candidate = 2;      /* next integer to test      */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    /* Generate terms until we have at least n terms */
    while (seq_len < n) {
        /* Find next prime */
        idx_t p = candidate;
        while (!is_prime(p)) ++p;

        /* Compute description and test primality of the description */
        seq_t descr = descr_from_prime(p);
        if (is_prime_big(descr)) {
            /* Add to memoised cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
                if (!seq_cache) return 0; /* allocation failure */
            }
            seq_cache[seq_len++] = (seq_t)p;
        }

        candidate = p + 1;
    }

    return seq_cache[n - 1];
}
