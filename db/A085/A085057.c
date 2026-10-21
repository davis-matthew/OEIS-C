/*
 * A085057  a(n) is the smallest integer of the form a*b*c.../p*q*r..., where
 *          the numerator and the denominator contain n numbers each and
 *          a,b,c,...p,q,r... are all the integers from 1 to 2n.
 * https://oeis.org/A085057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes p ≤ 2n that occur an odd number of
 *            times in the prime factorisation of (2n)!. This is the
 *            square‑free part of (2n)!. The implementation builds the prime
 *            sieve once, keeps a parity table for the exponents, and
 *            incrementally computes a(n) for all requested n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10000            /* maximum n expected (2n ≤ 20000) */
#define MAX_M (2 * MAX_N)      /* maximum 2n */

static bool prime_bool[MAX_M + 1];
static uint32_t prime_list[MAX_M / 2 + 1];
static size_t   prime_cnt       = 0;
static int      spf[MAX_M + 1];          /* smallest prime factor */
static bool     parity[MAX_M + 1];        /* exponent parity for each prime */
static seq_t   a_values[MAX_N + 1];       /* 1‑based results */
static bool initialized = false;
static idx_t   current_n = 0;             /* largest n whose value is known */

/* ------------------------------------------------------------------ */
/*  sieve primes up to MAX_M and fill spf[ ]                           */
static void sieve(void)
{
    for (int i = 0; i <= MAX_M; ++i) prime_bool[i] = (i >= 2);

    for (int i = 2; i <= MAX_M; ++i) {
        if (prime_bool[i]) {
            prime_list[prime_cnt++] = (uint32_t)i;
            spf[i] = i;
            if ((int64_t)i * i <= MAX_M) {
                for (int j = i * i; j <= MAX_M; j += i)
                    if (prime_bool[j]) {
                        prime_bool[j] = false;
                        spf[j] = i;
                    }
            }
        }
    }
    /* primes > sqrt(MAX_M) may lack spf entries */
    for (int i = 2; i <= MAX_M; ++i)
        if (prime_bool[i] && spf[i] == 0)
            spf[i] = i;
}

/* ------------------------------------------------------------------ */
/*  initialise static tables once                                         */
static void init(void)
{
    if (initialized) return;
    sieve();
    for (int i = 0; i <= MAX_M; ++i) parity[i] = false;
    for (size_t i = 0; i < prime_cnt; ++i) a_values[i + 1] = 0;
    a_values[0] = 0;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  extend precomputation to cover all n ≤ new_n                       */
static void extend_to_n(idx_t new_n)
{
    if (new_n <= current_n) return;

    idx_t start = 2 * current_n + 1;
    idx_t end   = 2 * new_n;          /* inclusive */

    for (idx_t num = start; num <= end; ++num) {
        idx_t x = num;
        /* toggle parity for each prime factor of x */
        while (x > 1) {
            uint32_t p = (uint32_t)spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (cnt & 1) parity[p] = !parity[p];
        }

        if ((num & 1ULL) == 0) {          /* even number → n = num/2 */
            idx_t n = num >> 1;
            seq_t prod = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = prime_list[i];
                if (p > num) break;
                if (parity[p]) prod *= p;
            }
            a_values[(size_t)n] = prod;
        }
    }
    current_n = new_n;
}

/* ------------------------------------------------------------------ */
/*  required external function                                          */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return 0;                 /* undefined for n=0, harmless return */
    extend_to_n(n);
    return a_values[(size_t)n];
}
