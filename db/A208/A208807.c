/*
 * A208807  Number of n-bead necklaces labeled with numbers -1..1 not allowing
 *          reversal, with sum zero and three times sum of squares <=
 *          n*(1)*(1+1).
 * https://oeis.org/A208807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): Number of n‑bead necklaces labeled with numbers -1,0,1
 * (no reversal), sum zero, and three times the sum of squares <= n*(1)*(1+1).
 * Formula: a(n) = (1/n) * sum_{d|n} φ(n/d) * g(d)
 * where φ is Euler's totient and g(d) counts linear strings of length d
 * with sum zero and at most 2d/3 non‑zero symbols. */
/* Author: ChatGPT */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* Utility: Euclidean gcd for 64‑bit integers (works in C and C++).           */
/* -------------------------------------------------------------------------- */
static uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* -------------------------------------------------------------------------- */
/* Binomial coefficient C(n,k) for unsigned integers n>=k using multiplicative */
/* formula: res = product_{i=1..k} (n-k+i)/i.                                */
/* -------------------------------------------------------------------------- */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
/* g(d): number of length‑d strings over {-1,0,1} with sum = 0 and at most   */
/* 2d/3 non‑zero symbols. Uses combinatorial sum over m where k=2m non‑zero.  */
/* -------------------------------------------------------------------------- */
static seq_t compute_g(idx_t d)
{
    seq_t total = (seq_t)0;
    idx_t max_m = d / 3;          /* floor(d/3)  */
    for (idx_t m = 0; m <= max_m; ++m) {
        seq_t ways1 = binom_uint(d, 2 * m);      /* choose positions of non‑zeros   */
        seq_t ways2 = binom_uint(2 * m, m);      /* choose which are +1              */
        total += ways1 * ways2;
    }
    return total;
}

/* -------------------------------------------------------------------------- */
/* Static tables for φ(n) and g(n).  They are resized as needed.              */
/* -------------------------------------------------------------------------- */
static idx_t _max_n = 0;
static unsigned int *phi_tbl = NULL;   /* φ values, up to _max_n           */
static seq_t *g_tbl = NULL;            /* g values, up to _max_n           */

/* Resize and recompute φ and g tables up to n. */
static void _ensure_tables(idx_t n)
{
    if (n <= _max_n) return;

    /* Allocate/resize arrays. */
    size_t sz = (size_t)(n + 1);
    unsigned int *new_phi = (unsigned int *)realloc(phi_tbl, sz * sizeof(unsigned int));
    seq_t *new_g = (seq_t *)realloc(g_tbl, sz * sizeof(seq_t));
    if (!new_phi || !new_g) return;   /* allocation failure – caller will return 0 */

    phi_tbl = new_phi;
    g_tbl = new_g;

    /* Initialise φ[i] = i, then sieve.                               */
    for (idx_t i = 1; i <= n; ++i) phi_tbl[i] = (unsigned int)i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_tbl[p] == (unsigned int)p) {            /* p is prime           */
            for (idx_t j = p; j <= n; j += p)
                phi_tbl[j] -= phi_tbl[j] / (unsigned int)p;
        }
    }

    /* Compute g(i) for all i up to n. */
    for (idx_t i = 1; i <= n; ++i) g_tbl[i] = compute_g(i);

    _max_n = n;
}

/* -------------------------------------------------------------------------- */
/* Public function required by the framework:  a(idx_t n)                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    _ensure_tables(n);

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;                            /* divisor pair         */
            seq_t term = (seq_t)(phi_tbl[nd]) * g_tbl[d];
            sum += term;
        }
    }
    return sum / (seq_t)n;
}
