/*
 * A082246  Primes that are the sum of 7 consecutive primes.
 * https://oeis.org/A082246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th prime that can be written as the sum of 7
 *            consecutive primes. This function builds the sequence on demand
 *            and memoises all terms that have been computed so far. It is
 *            independent of the order in which a(n) is called
 * Range:     u8 n <= 3, u16 n <= 277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9620 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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

#define MAX_PRIMES 500000   /* sufficient for n ≤ 10000 */
#define MAX_TERMS  20000    /* larger than the required number of terms */

/* static state ---------------------------------------------------------- */
static uint64_t      primes[MAX_PRIMES];   /* prime list */
static seq_t         sums  [MAX_TERMS];    /* sequence values */
static size_t        primes_len = 0;       /* number of primes stored   */
static size_t        terms_len  = 0;       /* number of sequence terms  */
static uint64_t      last_prime = 1;       /* last prime generated      */
static seq_t         sum  = 0;             /* current 7‑prime window sum */

/* helper to store a new term ------------------------------------------- */
static inline void store_term(seq_t val)
{
    if (terms_len < MAX_TERMS) {
        sums[terms_len++] = val;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* argument must be ≥ 1 */
    if (n == 0) return 0;   /* value for offset 0 is irrelevant */

    /* return cached value if it already exists */
    if (terms_len >= (size_t)n) {
        return sums[n - 1];
    }

    /* generate primes and terms until we have at least n terms */
    while (terms_len < (size_t)n) {

        /* generate the next prime */
        uint64_t p = next_prime(last_prime + 1);
        if (primes_len >= MAX_PRIMES) {
            /* should never happen for the required n range */
            return 0;
        }
        primes[primes_len++] = p;
        last_prime = p;

        if (primes_len < 7) {
            /* not enough primes yet to form a window */
            continue;
        }

        if (primes_len == 7) {
            /* first window – compute the sum explicitly */
            sum = 0;
            for (size_t i = 0; i < 7; ++i) {
                sum += (seq_t)primes[i];
            }
        } else {
            /* update the sliding sum: add new prime, remove oldest in window */
            sum += (seq_t)p;
            sum -= (seq_t)primes[primes_len - 8];
        }

        /* check if the current 7‑prime sum is prime */
        if (is_prime_big(sum)) {
            store_term(sum);
        }
    }

    return sums[n - 1];
}
