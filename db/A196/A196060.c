/*
 * A196060  The hyper-Wiener index of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A196060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = hyper-Wiener index of the rooted tree with Matula‑Goebel
 *            number n * It computes a(n) = (S(n) + S2(n))/2 where S is sum of
 *            pairwise distances * and S2 is sum of squared pairwise distances
 *            of that tree. Recursions are * performed on the factorisation of
 *            n into primes to get child indices. Me...
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions available from the build. */
#include "../utils/build_primes.h"

/* Static storage for recursion results. */
static idx_t cache_max = 0;

/* Arrays indexed by Matula‑Goebel number (1…cache_max) */
static seq_t *N   = NULL;   /* number of vertices in subtree */
static seq_t *D   = NULL;   /* sum of distances from root to all vertices */
static seq_t *D2  = NULL;   /* sum of squared distances from root to all vertices */
static seq_t *S   = NULL;   /* sum of distances between unordered pairs of vertices */
static seq_t *S2  = NULL;   /* sum of squared distances between unordered pairs of vertices */
static uint8_t *computed = NULL;   /* flag: 1 if value cached */

/* Prime list for mapping primes to their indices. */
static idx_t *prime_list = NULL;   /* array of primes (1‑based index) */
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure prime list up to some limit. */
static void ensure_primes(idx_t limit) {
    if (prime_list && limit <= prime_limit) return;
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Binary search for the 1‑based index of a prime p.  p is guaranteed to be prime. */
static idx_t prime_index(idx_t p) {
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* prime_list[lo] == p for valid input */
    if (lo >= prime_cnt) return 0;
    return (idx_t)(lo + 1);   /* convert 0‑based to 1‑based */
}

/* Resize cache arrays to be able to hold values up to n. */
static void ensure_cache(idx_t n) {
    if (cache_max >= n) return;

    /* free old cache if present */
    if (N)     free(N);
    if (D)     free(D);
    if (D2)    free(D2);
    if (S)     free(S);
    if (S2)    free(S2);
    if (computed) free(computed);

    size_t sz = (size_t)(n + 1);   /* 0…n inclusive */

    N = (seq_t *)calloc(sz, sizeof(seq_t));
    D = (seq_t *)calloc(sz, sizeof(seq_t));
    D2 = (seq_t *)calloc(sz, sizeof(seq_t));
    S = (seq_t *)calloc(sz, sizeof(seq_t));
    S2 = (seq_t *)calloc(sz, sizeof(seq_t));
    computed = (uint8_t *)calloc(sz, sizeof(uint8_t));

    /* base case: one‑vertex tree (n = 1) */
    if (n >= 1) {
        N[1] = 1;
        D[1] = 0;
        D2[1] = 0;
        S[1] = 0;
        S2[1] = 0;
        computed[1] = 1;
    }
    cache_max = n;
}

/* Recursive computation of subtree data for a given Matula‑Goebel number. */
static void compute_subtree(idx_t num) {
    if (num == 0) return;
    if (computed[num]) return;
    if (num == 1) {          /* safety; base case already stored in cache */
        computed[1] = 1;
        return;
    }

    /* Ensure prime list covers all primes that may appear as factors. */
    ensure_primes(num);

    /* Factorise num into primes and produce child indices. */
    idx_t temp = num;
    idx_t child_idx[32];
    size_t child_cnt = 0;

    for (size_t pi = 0; pi < prime_cnt && (uint64_t)prime_list[pi] * (uint64_t)prime_list[pi] <= temp; ++pi) {
        uint64_t p = (uint64_t)prime_list[pi];
        while (temp % p == 0) {
            child_idx[child_cnt++] = prime_index((idx_t)p);
            temp /= p;
        }
    }
    if (temp > 1) {
        /* remaining prime factor */
        child_idx[child_cnt++] = prime_index((idx_t)temp);
    }

    /* Compute data for all children first. */
    for (size_t i = 0; i < child_cnt; ++i) {
        compute_subtree(child_idx[i]);
    }

    /* Combine data from children. */
    seq_t Ntot  = 1;       /* root itself */
    seq_t Swith = 0;       /* sum within subtrees and cross pairs (excluding root‑vertex pairs) */
    seq_t S2with = 0;
    seq_t Droot = 0;       /* sum of distances from root to all vertices */
    seq_t D2root = 0;      /* sum of squared distances from root */

    for (size_t i = 0; i < child_cnt; ++i) {
        idx_t cid = child_idx[i];
        seq_t Ni = N[cid];
        seq_t Di = D[cid];
        seq_t Di2 = D2[cid];
        seq_t Si = S[cid];
        seq_t Si2 = S2[cid];

        Ntot += Ni;
        Swith += Si;
        S2with += Si2;
        Droot += Di + Ni;           /* Di is sum within subtree; add Ni for +1 */
        D2root += Di2 + 2 * Di + Ni;    /* (d+1)^2 = d^2 + 2d + 1, sum over Ni vertices gives +Ni */
    }

    /* Cross‑subtree contributions for S and S2. */
    for (size_t i = 0; i < child_cnt; ++i) {
        idx_t ci = child_idx[i];
        seq_t Ni = N[ci];
        seq_t Di = D[ci];
        seq_t Di2 = D2[ci];
        for (size_t j = i + 1; j < child_cnt; ++j) {
            idx_t cj = child_idx[j];
            seq_t Nj = N[cj];
            seq_t Dj = D[cj];
            seq_t Dj2 = D2[cj];

            /* distances between vertices in different subtrees */
            Swith += Di * Nj + Dj * Ni + 2 * Ni * Nj;

            /* squared distances between vertices in different subtrees */
            S2with += Di2 * Nj + Dj2 * Ni
                      + 2 * Di * Dj
                      + 4 * Di * Nj
                      + 4 * Dj * Ni
                      + 4 * Ni * Nj;
        }
    }

    /* Final totals for the subtree rooted at num. */
    seq_t S_total = Swith + Droot;
    seq_t S2_total = S2with + D2root;

    N[num] = Ntot;
    D[num] = Droot;
    D2[num] = D2root;
    S[num] = S_total;
    S2[num] = S2_total;
    computed[num] = 1;
}

/* Public function required by the exercise. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n=0, return 0 safely */
    ensure_cache(n);
    compute_subtree(n);
    return (S[n] + S2[n]) / 2;
}
