/*
 * A068231  Primes congruent to 11 mod 12.
 * https://oeis.org/A068231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 11 (mod 12) */
#include <stdlib.h>
#include <stdbool.h>

/* Ensure a seq_t array has capacity for at least 'need' elements. */
static void ensure_seq_capacity(seq_t **a, size_t *len, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t newcap = (*cap == 0) ? 64 : *cap * 2;
        if (newcap < need)
            newcap = need;
        *a = (seq_t *)realloc(*a, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Append a value to a seq_t array, resizing if needed. */
static void push_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    ensure_seq_capacity(arr, len, cap, *len + 1);
    (*arr)[*len] = val;
    (*len)++;
}

/* Return the n‑th prime congruent to 11 modulo 12 (a(1)=11). */
seq_t a(idx_t n)
{
    static seq_t *primes_all = NULL;   /* all primes found so far            */
    static size_t prime_cnt = 0, prime_cap = 0;
    static seq_t *mod12_primes = NULL; /* primes ≡ 11 (mod 12)              */
    static size_t mod12_cnt = 0, mod12_cap = 0;
    static seq_t candidate = 2;        /* next integer to test for primality */

    /* Keep generating primes until the required index is reached. */
    while (mod12_cnt < n) {
        bool is_prime = true;

        /* Trial division by the primes already found. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = primes_all[i];
            seq_t sq = p * p;
            if (sq > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            push_seq(&primes_all, &prime_cnt, &prime_cap, candidate);

            /* If it satisfies the residue condition, store it. */
            if ((candidate % 12) == 11)
                push_seq(&mod12_primes, &mod12_cnt, &mod12_cap, candidate);
        }

        ++candidate;
    }

    return mod12_primes[n - 1];
}
