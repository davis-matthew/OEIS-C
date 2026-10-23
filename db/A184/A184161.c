/*
 * A184161  Number of subtrees in the rooted tree with Matula-Goebel number n.
 * https://oeis.org/A184161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let b(n)=A184160(n) denote the number of subtrees of the rooted
 *            tree with Matula-Goebel number n that contain the root. Then
 *            a(1)=1; if n=prime(t) (the t-th prime), then a(n)=1+a(t)+b(t);
 *            if n=r*s (r,s≥2), then a(n)=a(r)+a(s)+(b(r)-1)*(b(s)-1)-1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/*  Prime table helpers (sieve on demand)                             */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* prime list and index (prime_index[p] = 1‑based index of prime p, 0 if not prime) */
static size_t prime_limit = 0;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static int *prime_index = NULL;

static void ensure_primes(size_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate new structures */
    size_t new_limit = limit;
    int *new_index = (int *)calloc(new_limit + 1, sizeof(int));
    if (!new_index) exit(EXIT_FAILURE);

    char *is_comp = (char *)calloc(new_limit + 1, sizeof(char));
    if (!is_comp) { free(new_index); exit(EXIT_FAILURE); }

    /* Simple sieve */
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Build prime list and index */
    size_t cap = 16;
    uint32_t *new_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!new_list) { free(is_comp); free(new_index); exit(EXIT_FAILURE); }
    size_t used = 0;

    for (size_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {
            if (used == cap) {
                cap <<= 1;
                new_list = (uint32_t *)realloc(new_list, cap * sizeof(uint32_t));
                if (!new_list) { free(is_comp); free(new_index); exit(EXIT_FAILURE); }
            }
            new_list[used++] = (uint32_t)i;
            new_index[i] = (int)(used);   /* 1‑based index */
        }
    }

    free(is_comp);

    /* Replace old tables */
    free(prime_index);
    free(prime_list);
    prime_index = new_index;
    prime_list  = new_list;
    prime_count = used;
    prime_limit = new_limit;
}

/* Smallest prime factor (returns n if n is prime) */
static size_t smallest_factor(uint64_t n)
{
    if (n < 2) return n;
    ensure_primes((size_t)n);
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return (size_t)p;
    }
    return (size_t)n;
}

/* ------------------------------------------------------------------ */
/*  Memoisation of a(n) and b(n)                                      */
/* ------------------------------------------------------------------ */
static seq_t *A_cache = NULL;
static seq_t *B_cache = NULL;
static size_t cache_cap = 0;

/* Grow cache arrays to at least (n+1) elements */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    A_cache = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    if (!A_cache) exit(EXIT_FAILURE);
    B_cache = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    if (!B_cache) exit(EXIT_FAILURE);

    /* Initialise new positions with zero */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        A_cache[i] = 0;
        B_cache[i] = 0;
    }
    cache_cap = new_cap;

    /* Base case */
    if (cache_cap > 1 && A_cache[1] == 0) {
        A_cache[1] = 1;
        B_cache[1] = 1;
    }
}

/* ------------------------------------------------------------------ */
/*  b(n) : number of subtrees containing the root                    */
/* ------------------------------------------------------------------ */
static seq_t compute_b(idx_t n)
{
    ensure_cache_size(n);
    if (B_cache[(size_t)n] != 0) return B_cache[(size_t)n];

    seq_t ret = 0;
    if (n == 1) {
        ret = 1;
    } else {
        ensure_primes((size_t)n);
        int idx = prime_index[(size_t)n];
        if (idx != 0) {                            /* n is prime */
            seq_t b_t = compute_b((idx_t)idx);
            ret = b_t + 1;
        } else {                                   /* composite */
            uint64_t temp = (uint64_t)n;
            seq_t prod = 1;
            for (size_t i = 0; i < prime_count; ++i) {
                uint32_t p = prime_list[i];
                if ((uint64_t)p * (uint64_t)p > temp) break;
                if (temp % p == 0) {
                    size_t child_idx = prime_index[p];
                    uint64_t count = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++count;
                    }
                    seq_t part = compute_b((idx_t)child_idx);
                    seq_t factor = part + 1;
                    /* multiply factor count times */
                    for (uint64_t k = 0; k < count; ++k) prod *= factor;
                }
            }
            if (temp > 1) {                         /* final prime factor */
                size_t child_idx = prime_index[(size_t)temp];
                seq_t part = compute_b((idx_t)child_idx);
                seq_t factor = part + 1;
                prod *= factor;
            }
            ret = prod;
        }
    }
    B_cache[(size_t)n] = ret;
    return ret;
}

/* ------------------------------------------------------------------ */
/*  a(n) : total number of subtrees in the tree of number n          */
/* ------------------------------------------------------------------ */
static seq_t compute_a(idx_t n)
{
    ensure_cache_size(n);
    if (A_cache[(size_t)n] != 0) return A_cache[(size_t)n];

    seq_t ret = 0;
    if (n == 1) {
        ret = 1;
    } else {
        ensure_primes((size_t)n);
        int idx = prime_index[(size_t)n];
        if (idx != 0) {                            /* prime case */
            seq_t a_t = compute_a((idx_t)idx);
            seq_t b_t = compute_b((idx_t)idx);
            ret = 1 + a_t + b_t;
        } else {                                   /* composite case */
            uint64_t r = smallest_factor((uint64_t)n);
            uint64_t s = (uint64_t)n / r;
            seq_t a_r = compute_a((idx_t)r);
            seq_t a_s = compute_a((idx_t)s);
            seq_t b_r = compute_b((idx_t)r);
            seq_t b_s = compute_b((idx_t)s);
            ret = a_r + a_s + (b_r - 1) * (b_s - 1) - 1;
        }
    }
    A_cache[(size_t)n] = ret;
    return ret;
}

/* ------------------------------------------------------------------ */
/*  Public entry point                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    return compute_a(n);
}
