/*
 * A187815  Number of permutations q_1, ..., q_7 of the 7 consecutive primes
 *          p_n, p_{n+1}, ..., p_{n+6} with q_1 = p_n and q_7 = p_{n+6}, and
 *          with |q_1-q_2|, |q_2-q_3|, ..., |q_6-q_7|, |q_7-q_1| pairwise
 *          distinct, where p_k denotes the k-th prime.
 * https://oeis.org/A187815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations q₁,…,q₇ of the 7 consecutive
 *            primes pₙ,…,pₙ₊₆ with q₁ = pₙ, q₇ = pₙ₊₆ and all absolute
 *            differences |qᵢ−qᵢ₊₁| (i=1..6) and |q₇−q₁| pairwise distinct
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- helpers ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* provides build_primes */

/* global cache of primes */
static uint64_t *g_primes = NULL;
static size_t   g_prime_cnt = 0;
static size_t   g_limit = 200000;

/* make sure we have at least idx_needed primes (0‑based count) */
static void
ensure_primes(idx_t n)
{
    size_t idx_needed = (size_t)n + 6;          /* need primes up to p_{n+6} */
    if (g_prime_cnt >= idx_needed)
        return;
    if (g_primes != NULL)
        free(g_primes);
    g_primes = NULL;
    g_prime_cnt = 0;
    while (g_prime_cnt < idx_needed) {
        build_primes(g_limit, &g_primes, &g_prime_cnt);
        if (g_prime_cnt >= idx_needed)
            break;
        g_limit <<= 1;                          /* double limit */
        free(g_primes);
    }
}

/* lexicographic next_permutation for array a[0..n-1] */
static bool
next_perm(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1])
        i--;
    if (i < 0)
        return false;
    int j = n - 1;
    while (a[j] <= a[i])
        j--;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    /* reverse suffix */
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
    }
    return true;
}

/* ---------- sequence entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0 but return 0 safely */
        return 0;

    /* make sure the prime array contains the block */
    ensure_primes(n);

    size_t idx0 = (size_t)n - 1;                 /* 0‑based first prime index */
    uint64_t p0 = g_primes[idx0];
    uint64_t p6 = g_primes[idx0 + 6];

    /* middle 5 primes */
    uint64_t mid[5];
    for (int i = 0; i < 5; ++i)
        mid[i] = g_primes[idx0 + 1 + i];

    int perm[5] = {0, 1, 2, 3, 4};
    size_t count = 0;

    do {
        uint64_t q[7];
        q[0] = p0;
        q[6] = p6;
        for (int k = 0; k < 5; ++k)
            q[k + 1] = mid[perm[k]];

        /* compute 7 absolute differences */
        int diff[7];
        for (int k = 0; k < 7; ++k) {
            uint64_t a0 = q[k];
            uint64_t a1 = q[(k + 1) % 7];
            diff[k] = a0 > a1 ? (int)(a0 - a1) : (int)(a1 - a0);
        }

        /* check all differences distinct */
        bool ok = true;
        for (int i = 0; i < 7 && ok; ++i)
            for (int j = 0; j < i; ++j)
                if (diff[i] == diff[j]) {
                    ok = false;
                    break;
                }
        if (ok)
            ++count;
    } while (next_perm(perm, 5));

    return (seq_t)count;
}
