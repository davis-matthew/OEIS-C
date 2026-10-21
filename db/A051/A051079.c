/*
 * A051079  Primes p such that x^20 = -2 has a solution mod p.
 * https://oeis.org/A051079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A051079 – the n‑th prime p for which x^20 ≡ –2 (mod p) has a
 *            solution
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

static seq_t *qvals = NULL;           /* list of qualifying primes */
static size_t qlen = 0, qcap = 0;
static uint64_t *primes = NULL;       /* list of all generated primes */
static size_t plen = 0, pcap = 0;
static uint64_t next_candidate = 3;   /* next odd number to test */
static bool initialized = false;

/*ensure capacity for qualifier list*/
static inline void ensure_qcap(void)
{
    if (qlen >= qcap) {
        size_t new_cap = qcap ? qcap * 2u : 8u;
        seq_t *tmp = (seq_t *)realloc(qvals, new_cap * sizeof(seq_t));
        qvals = tmp;
        qcap = new_cap;
    }
}

/*ensure capacity for prime list*/
static inline void ensure_pcap(void)
{
    if (plen >= pcap) {
        size_t new_cap = pcap ? pcap * 2u : 8u;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        pcap = new_cap;
    }
}

/* Euclid's algorithm for 64‑bit unsigned integers */

/* trial division using the primes list built so far */
static bool is_prime_trial(uint64_t n)
{
    if (n < 2u) return false;
    if (n == 2u) return true;
    if ((n & 1u) == 0u) return false;
    for (size_t i = 0u; i < plen; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0u) return false;
    }
    return true;
}

/* generate next prime after the current one */
static uint64_t next_prime(void)
{
    for (;;) {
        uint64_t cand = next_candidate;
        next_candidate += 2u;              /* skip even numbers */
        if (is_prime_trial(cand)) {
            ensure_pcap();
            primes[plen++] = cand;
            return cand;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        /* prime 2 is a qualifier */
        ensure_pcap();
        primes[plen++] = 2u;
        /* qualifier list starts with 2 */
        ensure_qcap();
        qvals[qlen++] = (seq_t)2u;
    }
    while (qlen < n) {
        uint64_t p = next_prime();
        if (p == 2u) { /* already accounted for */
            continue;
        }
        /* test whether –2 is a 20th power residue modulo p */
        uint64_t g = gcd_u64(20u, p - 1u);
        uint64_t exp = (p - 1u) / g;
        seq_t base = (seq_t)p - 2;          /* p – 2 ≡ –2 (mod p) */
        seq_t mod  = (seq_t)p;
        seq_t r = mod_pow(base, exp, mod);
        if (r == (seq_t)1u) {
            ensure_qcap();
            qvals[qlen++] = (seq_t)p;
        }
    }
    return qvals[n - 1u];
}
