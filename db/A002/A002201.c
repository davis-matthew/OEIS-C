/*
 * A002201  Superior highly composite numbers: positive integers n for which
 *          there is an e > 0 such that d(n)/n^e >= d(k)/k^e for all k > 1,
 *          where the function d(n) counts the divisors of n (A000005).
 * https://oeis.org/A002201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 2, a(n+1) = a(n)*p, where p is the prime that maximizes
 *            log(1 + 1/(k_p + 1)) / log(p), and k_p is the exponent of p in
 *            a(n). Each step lowers e past the next critical value, and the
 *            prime whose critical value it passes is multiplied in
 *            (Ramanujan's construction)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 37,
 *            big any n
 * Helpers:   sh_eps, sh_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/sh_eps.h"
#include "../utils/sh_isprime.h"

static uint64_t *s_choice = NULL;   /* prime multiplied in at each step */
static idx_t s_count = 0;
static idx_t s_cap = 0;

static uint64_t *s_primes = NULL;   /* primes used so far, plus the next unused one */
static unsigned *s_exp = NULL;
static long double *s_eps = NULL;
static size_t s_np = 0;             /* number of primes with exponent > 0 */
static size_t s_pcap = 0;

static void sh_ensure_pcap(size_t need) {
    if (need <= s_pcap) return;
    size_t nc = s_pcap ? s_pcap * 2 : 64;
    while (nc < need) nc *= 2;
    s_primes = (uint64_t *)realloc(s_primes, nc * sizeof(uint64_t));
    s_exp = (unsigned *)realloc(s_exp, nc * sizeof(unsigned));
    s_eps = (long double *)realloc(s_eps, nc * sizeof(long double));
    s_pcap = nc;
}

static void sh_extend(idx_t n) {
    if (s_primes == NULL) {
        sh_ensure_pcap(64);
        s_primes[0] = 2;
        s_exp[0] = 0;
        s_eps[0] = sh_eps(2, 0);
        s_np = 0;
    }
    if (n > s_cap) {
        idx_t nc = s_cap ? s_cap * 2 : 64;
        while (nc < n) nc *= 2;
        s_choice = (uint64_t *)realloc(s_choice, (size_t)nc * sizeof(uint64_t));
        s_cap = nc;
    }
    while (s_count < n) {
        /* candidates: indices 0..s_np (s_np is the next unused prime) */
        size_t best = 0;
        for (size_t i = 1; i <= s_np; i++)
            if (s_eps[i] > s_eps[best]) best = i;
        s_choice[s_count++] = s_primes[best];
        s_exp[best]++;
        s_eps[best] = sh_eps(s_primes[best], s_exp[best]);
        if (best == s_np) {
            s_np++;
            sh_ensure_pcap(s_np + 1);
            uint64_t q = s_primes[s_np - 1] + 1;
            while (!sh_isprime(q)) q++;
            s_primes[s_np] = q;
            s_exp[s_np] = 0;
            s_eps[s_np] = sh_eps(q, 0);
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    sh_extend(n);
    seq_t r = 1;
    for (idx_t i = 0; i < n; i++)
        r *= (seq_t)s_choice[i];
    return r;
}
