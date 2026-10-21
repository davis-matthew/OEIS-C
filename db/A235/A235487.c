/*
 * A235487  Self-inverse and multiplicative permutation of integers: For n <
 *          7, a(n)=n, a(7)=8 and a(8)=7, a(p_i) = p_{a(i)} for primes with
 *          index i <> 4, and for composites > 8, a(u*v) = a(u) * a(v).
 * https://oeis.org/A235487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Self‑inverse multiplicative permutation of integers defined by
 *            a(0)=0, a(1)=1..a(6)=6, a(7)=8, a(8)=7, a(2^(3k)) = 7^k,
 *            a(2^(3k+1)) = 2⋅7^k, a(2^(3k+2)) = 4⋅7^k, for odd primes p≠7:
 *            a(p)=p_{a(index(p))}, and a(u⋅v)=a(u)⋅a(v) for all u,v>1
 * Range:     u8 n <= 195, u16 n <= 9902, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   prime_pi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/prime_pi.h"
#include "../utils/seq_pow.h"

/* ---------- Prime tables ---------- */
static const idx_t PRIME_LIMIT = 200000;        /* should be large enough for a(i) in the required range */
static uint32_t *prime_list = NULL;            /* array of primes in order (0‑based) */
static size_t prime_count = 0;
static bool primes_ready = false;

/* Build the prime list once using a simple sieve. */
static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;

    bool *is_comp = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;                     /* allocation failure: leave empty */
    memset(is_comp, 0, (PRIME_LIMIT + 1) * sizeof(bool));

    prime_list = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    if (!prime_list) { free(is_comp); return; }

    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_count = cnt;
    free(is_comp);
}

/* Return the prime whose index (1‑based) is k.  Caller must guarantee k <= prime_count. */
static seq_t prime_at_index(idx_t k)
{
    if (!prime_list) init_primes();
    return (seq_t)prime_list[(size_t)(k - 1)];
}

/* ---------- Recursive computation of a(n) ---------- */
static seq_t a_calc(idx_t n);

/* Map a prime p (p > 7) to a(p) = p_{a(index(p))} */
static seq_t map_prime(idx_t p)
{
    if (p == 7) return (seq_t)8;            /* special case handled earlier */

    /* index of the prime p */
    idx_t idx = (idx_t)prime_pi((idx_t)p);  /* prime_pi returns seq_t */
    /* recursive evaluation of a(idx) */
    seq_t a_idx = a_calc(idx);

    /* idx conversion back to integer for prime lookup */
    idx_t pos = (idx_t)a_idx;
    /* In the intended range pos will be within our prime table */
    return prime_at_index(pos);
}

/* Factorial mapping for powers of two */
static seq_t power_of_two(idx_t exp)
{
    idx_t k = exp / 3U;
    idx_t r = exp % 3U;
    seq_t pow7k = seq_pow((seq_t)7, k);

    switch (r) {
        case 0: return pow7k;                 /* a(2^(3k))   = 7^k          */
        case 1: return pow7k * (seq_t)2;       /* a(2^(3k+1)) = 2⋅7^k         */
        default: return pow7k * (seq_t)4;      /* a(2^(3k+2)) = 4⋅7^k         */
    }
}

/* Main recursive routine */
static seq_t a_calc(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n < 7)  return (seq_t)n;      /* 2…6 */
    if (n == 7) return (seq_t)8;
    if (n == 8) return (seq_t)7;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* handle powers of two first */
    idx_t exp2 = 0;
    while ((temp & 1U) == 0) {        /* while even */
        temp >>= 1;
        ++exp2;
    }
    if (exp2 > 0)
        result *= power_of_two(exp2);

    /* factor remaining odd part with trial division */
    for (idx_t p = 3; p <= temp && p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t factor;
        if (p == 7)
            factor = seq_pow((seq_t)8, exp);          /* a(7^e) = 8^e */
        else
            factor = seq_pow(map_prime(p), exp);       /* a(p^e) = a(p)^e */

        result *= factor;
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t factor;
        if (temp == 7)
            factor = (seq_t)8;
        else
            factor = map_prime(temp);
        result *= factor;
    }

    return result;
}

/* External entry point */
seq_t a(idx_t n)
{
    return a_calc(n);
}
