/*
 * A165917  a(n) = sum of the squarefree quadratic non-residues of n.
 * https://oeis.org/A165917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 479, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the squarefree quadratic non‑residues of n. */
/* a(1)=0, a(2)=0; for n >= 3 the routine runs in O(n) time. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Static cache for smallest prime factors, up to the largest n seen. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure the SPF table covers 0..limit.  Larger tables replace smaller ones. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;          /* already large enough */

    if (spf != NULL) free(spf);              /* free old table */
    build_spf(limit, &spf);                  /* allocate and fill new table */
    spf_limit = limit;
}

/* Test whether x (<= n-1) is squarefree using the SPF table. */
static bool is_squarefree_uint64(uint64_t x)
{
    while (x > 1) {
        idx_t p = spf[x];
        uint64_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt++;
            if (cnt > 1) return false;       /* p^2 divides x */
        }
    }
    return true;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;             /* a(1) = a(2) = 0 */

    /* 1. Build SPF table up to n-1 for squarefree tests. */
    ensure_spf(n - 1);

    /* 2. Allocate and mark quadratic residues modulo n. */
    size_t len = (size_t)n;
    char *is_residue = (char *)calloc(len, sizeof *is_residue);
    if (is_residue == NULL) return (seq_t)0; /* allocation failure */

    for (idx_t x = 0; x < n; ++x) {
        idx_t sq = (idx_t)(((unsigned __int128)x * x) % n);
        is_residue[sq] = 1;
    }

    /* 3. Sum over squarefree quadratic non‑residues. */
    seq_t sum = (seq_t)0;
    for (idx_t a_val = 1; a_val < n; ++a_val) {
        if (!is_residue[a_val] && is_squarefree_uint64(a_val)) {
            sum += (seq_t)a_val;
        }
    }

    free(is_residue);
    return sum;
}
