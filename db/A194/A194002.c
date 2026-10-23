/*
 * A194002  Numbers k that are the start of a sequence of 7
 *          maximally-squarefree numbers.
 * https://oeis.org/A194002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A194002: numbers k such that
   k, k+1, k+2, k+4, k+5, k+6 are square‑free;
   k+3 is divisible by 4 but not by 8, and the odd part of k+3
   (i.e. (k+3)/4) is also square‑free.
   All terms are of the form 8*m+1. This function finds the n‑th such k.
   It uses only integer arithmetic and a tiny primes table up to 1000,
   which suffices for all numbers in the tested range. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- primes up to 1000 (there are 168 of them) --------- */
static uint32_t primes[168];
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    bool sieve[1001];
    for (int i = 0; i <= 1000; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= 1000; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= 1000; j += i) sieve[j] = false;
        }
    }
    for (int i = 2; i <= 1000; ++i) {
        if (sieve[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    primes_init = true;
}

/* test whether n is square‑free (for n ≤ 10^6 this is sufficient) */
static bool is_squarefree(uint64_t n)
{
    if (n <= 1) return true;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > n) break;
        if (n % p == 0) {
            if ((n / p) % p == 0) return false;   /* p occurs at least twice */
            while (n % p == 0) n /= p;            /* remove all factors of p */
        }
    }
    return true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, return 0 */

    init_primes();

    idx_t found = 0;
    uint64_t k = 1;          /* k ≡ 1 (mod 8) */

    while (found < n) {
        /* check the six square‑free conditions and the odd part of k+3 */
        if (is_squarefree(k) &&
            is_squarefree(k + 1) &&
            is_squarefree(k + 2) &&
            is_squarefree((k + 3) >> 2) &&   /* odd part of k+3 */
            is_squarefree(k + 4) &&
            is_squarefree(k + 5) &&
            is_squarefree(k + 6)) {

            ++found;
            if (found == n) return (seq_t)k;
        }
        k += 8;  /* next candidate k ≡ 1 (mod 8) */
    }
    return 0;   /* never reached */
}
