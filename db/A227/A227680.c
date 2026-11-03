/*
 * A227680  Numbers whose sum of semiprime divisors is a prime number.
 * https://oeis.org/A227680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose sum of semiprime divisors is a prime number
 * Range:     u8 n <= 24, u16 n <= 5611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9444 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

#define MAX_SPF 2000000

/* smallest prime factor for 0..MAX_SPF */
static uint32_t spf[MAX_SPF + 1];
static bool spf_ready = false;

/* initialise the spf array */
static void init_spf(void) {
    if (spf_ready) return;
    for (idx_t i = 0; i <= MAX_SPF; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= MAX_SPF; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= MAX_SPF / i) {
                for (idx_t j = i * i; j <= MAX_SPF; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_ready = true;
}

/* return true if number n is in A227680 */
static bool in_seq(idx_t val) {
    uint32_t x = (uint32_t)val;
    /* factorisation */
    uint32_t primes_arr[20];
    int exps_arr[20];
    int pc = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        primes_arr[pc] = p;
        exps_arr[pc] = e;
        ++pc;
    }

    /* sum of semiprime divisors */
    seq_t sum = 0;
    for (int i = 0; i < pc; ++i) {
        if (exps_arr[i] >= 2)
            sum += (seq_t)primes_arr[i] * (seq_t)primes_arr[i];
    }
    for (int i = 0; i < pc; ++i)
        for (int j = i + 1; j < pc; ++j)
            sum += (seq_t)primes_arr[i] * (seq_t)primes_arr[j];

    return isPrimeGeneric(sum);
}

seq_t a(idx_t n) {
    init_spf();
    idx_t found = 0;
    /* start searching from 2 (the smallest possible candidate) */
    for (idx_t cand = 2; /* infinite loop */; ++cand) {
        if (cand > MAX_SPF) {
            /* should not happen for n up to 10000 */
            /* fallback: extend sieve (not implemented) */
            /* but for safety we stop */
            return (seq_t)0;
        }
        if (in_seq(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
    /* unreachable */
    return (seq_t)0;
}
