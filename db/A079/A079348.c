/*
 * A079348  Fibonacci(p-(p/5)) is the not the first Fibonacci number that p
 *          divides.
 * https://oeis.org/A079348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 4098, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_mod_seq, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079348: primes p such that the rank of apparition of p does
   NOT equal p - (p/5).  The function a(n) returns the n-th such prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mod_mul.h"   /* mod_mul for uint64_t multiplication modulo */
#include "../utils/add_mod_seq.h"

static seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= 8) {
        return (seq_t)mod_mul((uint64_t)a, (uint64_t)b, (uint64_t)mod);
    }
    return (a * b) % mod;
}

/* fast doubling Fibonacci modulo mod; returns F(n) mod mod */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t a = 0, b = 1;
    if (n == 0) return a;

    /* find most significant bit of n */
    idx_t bit = ((idx_t)1) << (sizeof(idx_t) * 8 - 1);
    while ((n & bit) == 0) bit >>= 1;

    for (; bit; bit >>= 1) {
        /* c = a * (2*b - a) mod mod */
        seq_t t = add_mod_seq(b, b, mod);          /* 2*b mod mod */
        if (t >= a) t -= a; else t += mod - a;
        seq_t c = mul_mod_seq(a, t, mod);

        /* d = a*a + b*b mod mod */
        seq_t aa = mul_mod_seq(a, a, mod);
        seq_t bb = mul_mod_seq(b, b, mod);
        seq_t d = add_mod_seq(aa, bb, mod);

        if (n & bit) {
            a = d;
            seq_t tmp = add_mod_seq(c, d, mod);
            b = tmp;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* static storage for computed terms and primes */
    static seq_t *terms = NULL;
    static size_t term_cnt = 0, term_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint64_t cand = 2;          /* next candidate integer to test */

    while (term_cnt < n) {
        /* primality test using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            /* store the new prime */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = cand;

            /* compute Legendre symbol (cand/5) */
            int legendre = 0;
            uint64_t r = cand % 5;
            if (r == 1 || r == 4) legendre = 1;
            else if (r == 2 || r == 3) legendre = -1;
            /* cand == 5 gives r==0 -> legendre stays 0 */

            uint64_t d = cand - (uint64_t)legendre;    /* d = p - (p/5) */

            /* factor d using available primes */
            uint64_t pf[64], ex[64];
            size_t pf_cnt = 0;
            uint64_t temp = d;
            for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
                uint64_t q = primes[i];
                if (temp % q == 0) {
                    uint64_t e = 0;
                    do { temp /= q; ++e; } while (temp % q == 0);
                    pf[pf_cnt] = q;
                    ex[pf_cnt] = e;
                    ++pf_cnt;
                }
            }
            if (temp > 1) {
                pf[pf_cnt] = temp;
                ex[pf_cnt] = 1;
                ++pf_cnt;
            }

            /* enumerate all divisors of d */
            uint64_t divs[1024];
            size_t div_cnt = 1;
            divs[0] = 1;
            for (size_t i = 0; i < pf_cnt; ++i) {
                size_t cur = div_cnt;
                uint64_t factor = pf[i];
                for (uint64_t e = 1; e <= ex[i]; ++e) {
                    for (size_t j = 0; j < cur; ++j) {
                        divs[div_cnt++] = divs[j] * factor;
                    }
                    factor *= pf[i];
                }
            }
            if (div_cnt > 1) qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

            /* find the minimal divisor m where F(m) ≡ 0 (mod cand) */
            uint64_t rank = d;   /* default to full d if none found earlier */
            for (size_t i = 0; i < div_cnt; ++i) {
                uint64_t m = divs[i];
                seq_t fm = fib_mod((idx_t)m, (seq_t)cand);
                if (fm == 0) { rank = m; break; }
            }

            if (rank != d) {
                /* this prime is part of the sequence */
                if (term_cnt == term_cap) {
                    size_t new_cap = term_cap ? term_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    terms = tmp;
                    term_cap = new_cap;
                }
                terms[term_cnt++] = (seq_t)cand;
            }
        }

        /* advance to next candidate */
        if (cand == 2) cand = 3;
        else cand += 2;
    }

    return terms[n - 1];
}
