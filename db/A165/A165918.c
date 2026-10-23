/*
 * A165918  a(n) = number of squarefree quadratic non-residues of n.
 * https://oeis.org/A165918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squarefree quadratic non-residues modulo n
 * Range:     u8 n <= 439, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spfs = NULL;
static idx_t spf_limit = 0;

/* Ensure smallest prime factor table up to `limit` */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    free(spfs);
    spfs = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    for (idx_t i = 2; i <= limit; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    if (spfs[j] == 0) spfs[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Test whether x is squarefree using the SPF table */
static bool is_squarefree(idx_t x)
{
    if (x <= 1) return true;
    while (x > 1) {
        uint32_t p = spfs[x];
        if (p == 0) return false;                 /* safety */
        x /= p;
        if (x % p == 0) return false;             /* p^2 divides x */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(1) = 0 by definition */

    /* Build SPF table up to n */
    ensure_spf(n + 100);             /* a small buffer to avoid re‑building */

    /* Determine set of quadratic residues modulo n */
    char *residues = (char*)calloc(n + 1, sizeof(char));
    if (!residues) return 0;        /* fallback on allocation failure */

    for (idx_t x = 0; x < n; ++x) {
        idx_t sq = (x * x) % n;      /* n is small; no overflow */
        residues[sq] = 1;
    }

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {          /* k = n is automatically 0 mod n (residue) */
        if (is_squarefree(k)) {
            if (!residues[k]) ++count;        /* non‑residue */
        }
    }

    free(residues);
    return count;
}
