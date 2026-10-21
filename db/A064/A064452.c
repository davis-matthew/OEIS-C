/*
 * A064452  Integers which are not primes or powers of primes such that for
 *          all j>i Phi(j)/j > Phi(i)/i.
 * https://oeis.org/A064452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) consists of the n‑th integer >1 that
 * is neither a prime nor a prime power and satisfies
 * φ(j)/j > φ(i)/i for all j > i, where φ is Euler's totient.
 * This implementation generates the terms by testing integers
 * in increasing order and keeps the best ratio seen so far. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime list, built incrementally ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint32_t prime_upto = 0;

static void ensure_primes_up_to(uint32_t limit)
{
    if (limit <= prime_upto)
        return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }
    if (prime_cnt == 0) {          /* seed 2 */
        prime_list[prime_cnt++] = 2;
        prime_upto = 2;
    }

    for (uint32_t x = prime_upto + 1; x <= limit; ++x) {
        bool isp = true;
        uint32_t r = (uint32_t)sqrt((double)x);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (x % prime_list[i] == 0) {
                isp = false;
                break;
            }
        }
        if (isp) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = x;
        }
    }
    prime_upto = limit;
}

/* ---------- Factorisation and φ computation ---------- */
static void factor_phi(uint64_t n, uint64_t *phi, size_t *distinct)
{
    uint64_t t = n;
    *phi = 1;
    *distinct = 0;
    uint32_t limit = (uint32_t)sqrt((double)t) + 1;
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > t)
            break;
        if (t % p == 0) {
            ++(*distinct);
            uint64_t mul = p - 1;
            t /= p;
            while (t % p == 0) {
                mul *= p;
                t /= p;
            }
            *phi *= mul;
        }
    }
    if (t > 1) {          /* remaining prime factor */
        ++(*distinct);
        *phi *= t - 1;
    }
}

/* ---------- Sequence storage and generation ---------- */
static seq_t *term_table  = NULL;
static size_t term_len    = 0;
static size_t term_cap    = 0;

static uint64_t best_phi = 0; /* φ of the last buffered term */
static uint64_t best_n   = 1;  /* n of the last buffered term */

static uint64_t candidate = 2; /* next integer to examine */

static void generate_next_term(void)
{
    while (1) {
        uint64_t n = candidate++;
        uint64_t phi;
        size_t distinct;
        factor_phi(n, &phi, &distinct);
        if (distinct <= 1)          /* prime or prime power */
            continue;

        /* φ(n)/n > best_phi/best_n?  use cross‑multiplication */
        if ((__int128)phi * best_n > (__int128)best_phi * n) {
            if (term_len == term_cap) {
                size_t newcap = term_cap ? term_cap * 2 : 8;
                term_table = (seq_t *)realloc(term_table,
                                              newcap * sizeof(seq_t));
                term_cap   = newcap;
            }
            term_table[term_len++] = (seq_t)n;
            best_phi = phi;
            best_n   = n;
            return;
        }
    }
}

/* ---------- Public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, but keep safe */
    while (term_len < (size_t)n)
        generate_next_term();
    return term_table[(size_t)n - 1];
}
