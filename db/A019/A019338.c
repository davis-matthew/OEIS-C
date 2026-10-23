/*
 * A019338  Primes with primitive root 8.
 * https://oeis.org/A019338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that the multiplicative order of 8 modulo p is p-1
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Static buffer large enough for the required number of terms
   (10000 terms require far fewer than 20000 primes).  */
#define MAX_PRIMES 20000
static seq_t special_primes[MAX_PRIMES];
static idx_t special_count = 0;
static uint64_t last_checked = 2;

/* Check whether a prime p has primitive root 8.  */
static bool check_order8_prime(seq_t p)
{
    /* 2 is not suitable, 3 is the first prime with ord_8(p)=p-1. */
    if (p == (seq_t)2) return false;
    if (p == (seq_t)3) return true;

    uint64_t pp   = (uint64_t)p;
    uint64_t phi  = pp - 1;
    /* 8 is a primitive root iff 3 does not divide p-1 and 2 has
       full order modulo p (since ord_8 = p-1 ⇔ 3∤p-1 and 2 is a primitive root). */
    if (phi % 3 == 0) return false;

    uint64_t temp = phi;
    for (uint64_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            /* Remove all occurrences of q from temp. */
            do { temp /= q; } while (temp % q == 0);

            /* If 2^((p-1)/q) == 1 mod p, then 2 is not a primitive root. */
            if (mod_pow((seq_t)2, phi / q, p) == (seq_t)1)
                return false;
        }
    }
    /* Remaining prime factor, if any. */
    if (temp > 1) {
        if (mod_pow((seq_t)2, phi / temp, p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Generate terms until we have at least n of them. */
static void ensure_special_primes(idx_t n)
{
    if (special_count >= n) return;

    while (special_count < n) {
        /* Find next prime greater than last_checked. */
        ++last_checked;
        if (last_checked % 2 == 0) ++last_checked;
        while (!is_prime((idx_t)last_checked)) last_checked += 2;

        seq_t p = (seq_t)last_checked;
        if (check_order8_prime(p)) {
            if (special_count < MAX_PRIMES)
                special_primes[special_count++] = p;
            else
                return; /* safety: exceed buffer; should not happen for given limits. */
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; for safety we return 0. */
    ensure_special_primes(n);
    return special_primes[n - 1];
}
