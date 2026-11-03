/*
 * A215313  Primes congruent to {1, 2, 3, 4, 5} mod 19.
 * https://oeis.org/A215313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 939 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to {1,2,3,4,5} mod 19 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: grow allowed-primes array */
static void grow_allowed(size_t needed)
{
    size_t newcap = needed ? needed * 2 : 4;
    seq_t *newptr = mem_resize(seq_t, NULL, newcap);
    /* On first use realloc works the same as allocate */
    newptr = mem_resize(seq_t, NULL, newcap);
    if (newptr == NULL) exit(1);
    newptr = mem_resize(seq_t, NULL, newcap);
    /* Actually we need to keep previous data – use realloc directly on existing */
    newptr = mem_resize(seq_t, NULL, newcap);
    /* The above line is a no-op placeholder; see below code's use. */
}

static void grow_allowed_check(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap) {
        size_t newcap = *cap ? *cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
        if (tmp == NULL) exit(1);
        *arr = tmp;
        *cap = newcap;
    }
}

/* Helper: grow primes list array */
static void grow_primes_check(size_t needed, size_t *cap, uint32_t **arr)
{
    if (needed > *cap) {
        size_t newcap = *cap ? *cap * 2 : 4;
        uint32_t *tmp = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
        if (tmp == NULL) exit(1);
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *allowed = NULL;
    static size_t allowed_cap = 0;
    static idx_t allowed_cnt = 0;

    static uint32_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t primes_cnt = 0;

    static uint32_t curr = 0;   /* current candidate to test for primality */

    /* Ensure we have at least n allowed primes */
    while (allowed_cnt < n) {
        /* Start with 2 on first call */
        if (curr == 0) curr = 2;

        bool is_prime = true;

        /* Trial division using stored primes */
        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > curr) break;
            if (curr % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store prime in primes list */
            grow_primes_check(primes_cnt + 1, &primes_cap, &primes);
            primes[primes_cnt++] = curr;

            /* check the residue modulo 19 */
            uint32_t r = curr % 19;
            if (r >= 1 && r <= 5) {
                grow_allowed_check(allowed_cnt + 1, &allowed_cap, &allowed);
                allowed[allowed_cnt++] = (seq_t)curr;
            }
        }

        /* advance to next candidate */
        ++curr;
        while (curr % 2 == 0) ++curr;  /* skip even numbers after 2 */
    }

    return allowed[n - 1];
}
