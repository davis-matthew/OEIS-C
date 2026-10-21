/*
 * A095392  Numbers n such that more than half of the reduced-residue system
 *          modulo 210 consists of primes in the following sense: in {210n +
 *          R} more than 24 = phi(210)/2 primes occur, i.e., 25-33, 35, 46.
 * https://oeis.org/A095392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that more than half (i.e. >24) of
 *            the numbers 210*k+r, where r runs over the reduced residue
 *            system modulo 210, are prime
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----------------------------------- includes -----------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd16.h"

/*----------------------------- reduced residue set mod 210 -----------------------*/
#define RESIDUE_COUNT 48
static uint16_t residues[RESIDUE_COUNT];
static bool residues_inited = false;

static void init_residues(void)
{
    if (residues_inited)
        return;
    int cnt = 0;
    for (uint16_t r = 1; r < 210; ++r) {
        if (gcd16(r, 210) == 1) {
            residues[cnt++] = r;
        }
    }
    residues_inited = true;
}

/*--------------------------- sieve of Eratosthenes (dynamic) -------------------*/
static unsigned char *sieve = NULL;      /* 0 = composite, 1 = prime */
static size_t sieve_size = 0;            /* currently allocated size */

static void build_sieve(size_t limit)
{
    if (sieve != NULL)
        free(sieve);
    sieve = (unsigned char*)malloc(limit + 1);
    if (sieve == NULL)
        exit(1);                       /* allocation failure */
    for (size_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                sieve[j] = 0;
        }
    }
    sieve_size = limit;
}

static void ensure_sieve(size_t needed)
{
    if (needed <= sieve_size)
        return;
    size_t new_limit = sieve_size < 2 ? 2 : sieve_size;
    while (new_limit < needed)
        new_limit <<= 1;
    build_sieve(new_limit);
}

/*--------------------------------- main function --------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_residues();

    idx_t found = 0;        /* number of qualifying n's seen so far */
    idx_t cand   = 0;        /* current candidate n */

    /* generate successive candidates until we reach the n-th qualifying one */
    for (;;) {
        ++cand;
        /* make sure the sieve covers all numbers 210*cand+r (r ≤ 209) */
        size_t max_needed = (size_t)210ULL * cand + 209;
        ensure_sieve(max_needed);

        int prime_cnt = 0;
        for (int i = 0; i < RESIDUE_COUNT; ++i) {
            size_t val = (size_t)210ULL * cand + (size_t)residues[i];
            if (sieve[val])
                ++prime_cnt;
        }
        if (prime_cnt > 24) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
