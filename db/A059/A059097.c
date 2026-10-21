/*
 * A059097  Numbers k such that the binomial coefficient C(2k,k) is not
 *          divisible by the square of an odd prime.
 * https://oeis.org/A059097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059097: a(n) = the n‑th non‑negative integer k such that
   the central binomial coefficient C(2k,k) is not divisible by p²
   for any odd prime p.  The function generates the terms on‑demand
   keeping a cache of all values computed so far. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* --- prime generator ----------------------------------------------------- */
static uint64_t *prime_list = NULL;          /* dynamic array of primes   */
static size_t   prime_cap  = 0;
static size_t   prime_len  = 0;
static uint64_t prime_last = 1;              /* last integer checked for primality */

/* Ensure prime_list holds all primes ≤ limit.  The list grows
   dynamically as needed. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_last)
        return;
    for (uint64_t n = prime_last + 1; n <= limit; ++n) {
        bool isprime = (n >= 2);
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            /* p*p may exceed 64‑bit, use 128‑bit to compare */
            if ((unsigned __int128)p * p > n)
                break;
            if (n % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) exit(1); /* allocation failure */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_len++] = n;
        }
    }
    prime_last = limit;
}

/* ----------------------------------------------------------------------- */

/* --- table of sequence values ------------------------------------------- */
static seq_t *seq_table = NULL;              /* dynamic array of seq_t values */
static size_t  seq_cap   = 0;
static size_t  seq_len   = 0;
static uint64_t next_k   = 0;                /* candidate k to test next */

/* Ensure the value table can hold 'need' entries. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_table, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_table = tmp;
    seq_cap   = new_cap;
}

/* ----------------------------------------------------------------------- */

/* Test whether k satisfies the condition.  For each odd prime p ≤ k
   compute the exponent of p in C(2k,k) using Legendre’s formula.
   If the exponent is ≥ 2 for any prime, k is rejected. */
static bool good_k(uint64_t k)
{
    if (k == 0)        /* C(0,0)=1 is always square‑free */
        return true;

    ensure_primes_upto(k);

    for (size_t i = 0; i < prime_len && prime_list[i] <= k; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)    /* only odd primes matter */
            continue;

        /* Legendre's exponent: v_p((2k)!) – 2·v_p(k!) */
        unsigned long long exp_k  = 0;
        unsigned long long exp_2k = 0;

        unsigned __int128 t = k;
        while (t) { t /= p; exp_k += (unsigned long long)t; }

        t = (unsigned __int128)k * 2;
        while (t) { t /= p; exp_2k += (unsigned long long)t; }

        unsigned long long exp = exp_2k - 2 * exp_k;
        if (exp >= 2)
            return false;
    }
    return true;
}

/* ----------------------------------------------------------------------- */

/* Generate sequence terms until we have at least 'need' entries. */
static void generate_to(idx_t need)
{
    if (need == 0)
        return;
    ensure_seq_cap((size_t)need);
    while (seq_len < (size_t)need) {
        if (good_k(next_k))
            seq_table[seq_len++] = (seq_t)next_k;
        ++next_k;
    }
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1 – return 0 safely */
    generate_to(n);
    return seq_table[n - 1];
}
