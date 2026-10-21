/*
 * A058213  Triangular arrangement of solutions of phi(x) = 2^n (n >= 0),
 *          where phi=A000010 is Euler's totient function. Each row
 *          corresponds to a particular n and its length is n+2 for 0 <= n <=
 *          31, 32 for n >= 32. (This assumes that there are only 5 Fermat
 *          primes.)
 * https://oeis.org/A058213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = element at index n in the triangular arrangement of *
 *            solutions of φ(x) = 2^m, where each row m has m+2 terms for m ≤
 *            31 * and 32 terms for m ≥ 32
 * Range:     u8 n <= 34, u16 n <= 134, u32 n <= 526, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation description:
 * 1. Determine the row m that contains the global index n.
 * 2. Enumerate all 32 subsets of the five known Fermat primes
 *    (3, 5, 17, 257, 65537).  For each subset we have:
 *          e      = sum of the corresponding exponents {1,2,4,8,16}
 *          d      = product of the chosen Fermat primes
 *    If e < m, there is exactly one solution:
 *          x = d * 2^(m - e + 1)
 *    If e = m, there are two solutions:
 *          odd   x = d
 *          even  x = d * 2
 *    For m ≥ 32 (where e ≤ 31 < m) we fall into the first case for every
 *    subset, giving exactly 32 solutions.
 * 3. Store the solutions in an array, sort them, and return the offset
 *    within the row.  Sorting is performed by a simple comparison
 *    routine suitable for any unsigned seq_t type. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Pre‑computed Fermat primes and their product table. */
static const seq_t fermat_primes[5] = {3, 5, 17, 257, 65537};
static seq_t prod_mask[32];
static bool   prod_inited = false;

/* Initialises prod_mask[mask] = product of Fermat primes encoded in mask. */
static void init_prod_mask(void)
{
    for (int mask = 0; mask < 32; ++mask) {
        seq_t v = 1;
        for (int bit = 0; bit < 5; ++bit) {
            if (mask & (1u << bit))
                v *= fermat_primes[bit];
        }
        prod_mask[mask] = v;
    }
    prod_inited = true;
}

/* Comparator for qsort to work with arbitrary seq_t. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t aa = *(const seq_t *)a;
    seq_t bb = *(const seq_t *)b;
    if (aa < bb) return -1;
    if (aa > bb) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Initialise product table once. */
    if (!prod_inited)
        init_prod_mask();

    /* --- Find the row m that contains index n. --- */
    idx_t m;            /* row index (m = φ exponent) */
    idx_t prev_total;   /* number of elements before this row */

    if (n < 560) { /* rows 0 … 31 */
        m = 0;
        while ((m * (m + 3) / 2) <= n)
            ++m;          /* now (m*(m+3))/2 > n */
        --m;                 /* so m satisfies C(m) ≤ n < C(m+1) */
        prev_total = (m * (m + 3)) / 2;
    } else {          /* rows 32 and above */
        m = 32 + (n - 560) / 32;
        prev_total = 560 + (m - 32) * 32;
    }

    idx_t offset = n - prev_total;          /* position inside the row */

    /* --- Generate all solutions for row m. --- */
    seq_t solutions[64];
    size_t sol_cnt = 0;

    for (uint32_t mask = 0; mask < 32; ++mask) {
        uint8_t e = (uint8_t)mask;           /* sum of exponents */
        if (m <= 31 && e > m)                /* only consider e ≤ m for m ≤ 31 */
            continue;

        seq_t d = prod_mask[mask];

        if (m <= 31) {                       /* small rows with odd solutions */
            if (e < m) {                     /* single even solution */
                solutions[sol_cnt++] = d * pow2((idx_t)(m - e + 1));
            } else if (e == m) {              /* odd and even solutions */
                solutions[sol_cnt++] = d;                     /* odd */
                solutions[sol_cnt++] = d * pow2(1);           /* even */
            }
        } else {                             /* rows 32 and above */
            /* e is always < m (e ≤ 31 < m). One even solution. */
            solutions[sol_cnt++] = d * pow2((idx_t)(m - e + 1));
        }
    }

    /* Sort solutions and return the requested element. */
    qsort(solutions, sol_cnt, sizeof(seq_t), cmp_seq);
    return solutions[(size_t)offset];
}
