/*
 * A078861  Least positive residues [mod 210] representing those residue
 *          classes which can be smaller prime of a difference 6 taken from
 *          A031924.
 * https://oeis.org/A078861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = least positive residue modulo 210 of a smaller prime p
 *            such that p+6 is also prime. The sequence consists of the 30
 *            residue classes r (1 ≤ r < 210) for which there exists at least
 *            one prime p ≡ r (mod 210) with p and p+6 prime. a(1)=11,
 *            a(2)=13, … (first few terms shown in OEIS)
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   cmp_uint, gcd_uint, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrime.h"   /* isPrime(idx_t n) */
#include "../utils/cmp_uint.h"
#include "../utils/gcd_uint.h"

/* Stored residues (in ascending order).  The sequence has at most 30 terms. */
static unsigned int residues[30];
static size_t residues_cnt = 0;
static bool residues_initialized = false;

/* Helper arrays for the algorithm. */
static bool     allowed[210];          /* marks the 30 allowed residue classes   */
static unsigned int allowed_res[30];   /* list of those residue classes           */
static bool     seen[210];             /* marks which residues we already found  */
static unsigned int allowed_cnt = 0;

/* Precompute residues and find all 30 that actually occur. */
static void init_residues(void)
{
    if (residues_initialized) return;

    /* --------------------------------------------------------------------
     * 1. Find the 30 residue classes r in [1,210) with gcd(r,210)=1 and
     *    gcd(r+6,210)=1.  These are the only candidates that can produce
     *    a sexy‑prime pair (p,p+6) with p≡r (mod 210).
     * -------------------------------------------------------------------- */
    memset(allowed, 0, sizeof(allowed));
    for (unsigned int r = 1; r < 210U; ++r) {
        if (gcd_uint(r, 210U) == 1U &&
            gcd_uint((r + 6U) % 210U, 210U) == 1U) {
            allowed[r] = true;
            allowed_res[allowed_cnt++] = r;
        }
    }

    /* --------------------------------------------------------------------
     * 2.  Sieve primes up to a modest limit (20 000 000) and collect the
     *     residues for which a prime pair exists.
     * -------------------------------------------------------------------- */
    const unsigned int LIMIT = 20000000U;
    unsigned char *sieve = (unsigned char *)malloc((LIMIT + 1U) * sizeof(unsigned char));
    if (!sieve) return;  /* failure unlikely on typical systems */
    memset(sieve, 1U, LIMIT + 1U);
    sieve[0] = sieve[1] = 0U;
    for (unsigned int i = 2U; (unsigned long long)i * i <= (unsigned long long)LIMIT; ++i) {
        if (sieve[i]) {
            for (unsigned long long j = (unsigned long long)i * i; j <= LIMIT; j += i)
                sieve[j] = 0U;
        }
    }

    /* Scan primes and record suitable residues */
    for (unsigned int p = 2U; p + 6U <= LIMIT; ++p) {
        if (sieve[p] && sieve[p + 6U]) {
            unsigned int r = p % 210U;
            if (allowed[r] && !seen[r]) {
                residues[residues_cnt++] = r;
                seen[r] = true;
                if (residues_cnt == 30U) break;
            }
        }
    }
    free(sieve);

    /* --------------------------------------------------------------------
     * 3.  For any residue not yet seen, search further using the
     *     deterministic 64‑bit primality test to guarantee completeness.
     * -------------------------------------------------------------------- */
    for (unsigned int i = 0; i < allowed_cnt; ++i) {
        unsigned int r = allowed_res[i];
        if (seen[r]) continue;
        /* Incrementally test p = r + 210*k.  Stop when a pair is found. */
        for (unsigned long long k = 0; k < 2000000ULL; ++k) {
            unsigned long long p = (unsigned long long)210U * k + r;
            if (!isPrime(p)) continue;
            if (isPrime(p + 6ULL)) {
                residues[residues_cnt++] = r;
                seen[r] = true;
                break;
            }
        }
    }

    /* Finally, sort the residues so that a(1) is the smallest. */
    if (residues_cnt > 1U) {
        qsort(residues, residues_cnt, sizeof(unsigned int), cmp_uint);
    }

    residues_initialized = true;
}

/* --------------------------------------------------------------
   Public function:  seq_t a(idx_t n)
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* the sequence is 1‑indexed */
    init_residues();
    if ((size_t)n > residues_cnt) return 0;   /* beyond known terms */
    return (seq_t)residues[(size_t)n - 1U];
}
