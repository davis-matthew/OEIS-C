/*
 * A129823  Quadruples (k,k+2,k+4,k+6) such that k is odd and k, k+2, k+4, and
 *          k+6 are composite.
 * https://oeis.org/A129823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   the sequence lists all odd k with k,k+2,k+4,k+6 composite. Each
 *            quadruple is output in overlapping fashion: (k,k+2,k+4,k+6),
 *            (k+2,k+4,k+6,k+8), … and the nth term a(n) is the (n‑th) element
 *            of that list
 * Range:     u8 n <= 40, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------ primality test for 64‑bit numbers ------------------ */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes for quick trial division */
    static const uint64_t primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof primes / sizeof primes[0]; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __int128 x = 1, base = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * base) % n;
            base = (base * base) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------------- dynamic array helpers --------------------------- */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ---------------------------- sequence ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety – OEIS offset is 1 */

    /* static state to memoize */
    static size_t cache_len   = 0;     /* number of elements stored   */
    static seq_t *cache       = NULL;  /* sequence array              */
    static size_t cache_cap  = 0;      /* allocated capacity          */
    static uint64_t next_k    = 3;     /* next odd candidate          */

    size_t wanted = (size_t)n;        /* target length */

    while (cache_len < wanted) {
        /* Search for the next odd k such that k,k+2,k+4,k+6 are all composite */
        uint64_t k = next_k;
        while (true) {
            if (!is_prime_64(k)     &&
                !is_prime_64(k + 2) &&
                !is_prime_64(k + 4) &&
                !is_prime_64(k + 6)) break;
            k += 2; /* next odd */
        }

        /* Append the found quadruple */
        ensure_capacity(cache_len + 4, &cache_cap, &cache);
        cache[cache_len++] = (seq_t)k;
        cache[cache_len++] = (seq_t)(k + 2);
        cache[cache_len++] = (seq_t)(k + 4);
        cache[cache_len++] = (seq_t)(k + 6);

        /* Prepare for the next search – allow overlapping quadruples */
        next_k = k + 2;
    }

    /* Return the required term (indexing from 1) */
    return cache[wanted - 1];
}
