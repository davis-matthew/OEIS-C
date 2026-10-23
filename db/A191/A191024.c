/*
 * A191024  Primes that are squares mod 31.
 * https://oeis.org/A191024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth prime that is a quadratic residue modulo 31.
   The residues modulo 31 that are squares (excluding 0) are:
   1,2,4,5,7,8,9,10,14,16,18,19,20,25,28. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: grow an array of type T. */
static void grow_array(void **ptr, size_t *size, size_t *cap, size_t elem_size)
{
    if (*cap == 0) {
        *cap = 64;
    } else {
        *cap <<= 1;
    }
    *ptr = realloc(*ptr, *cap * elem_size);
}

/* Global state for caching results and primes. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t *prime_list = NULL;
static size_t prime_len = 0, prime_cap = 0;
static idx_t next_candidate = 2;

/* Residue table: isResidue[r] == true iff r is a non‑zero square mod 31. */
static const bool isResidue[31] = {
    /*0*/ false,
    /*1*/ true,  /*2*/ true,  /*3*/ false,  /*4*/ true,  /*5*/ true,  /*6*/ false,
    /*7*/ true,  /*8*/ true,  /*9*/ true,  /*10*/ true, /*11*/ false, /*12*/ false,
    /*13*/ false, /*14*/ true, /*15*/ false, /*16*/ true, /*17*/ false, /*18*/ true,
    /*19*/ true,  /*20*/ true,  /*21*/ false, /*22*/ false, /*23*/ false, /*24*/ false,
    /*25*/ true,  /*26*/ false, /*27*/ false, /*28*/ true,  /*29*/ false, /*30*/ false
};

/* Append prime to prime list. */
static void add_prime(idx_t p)
{
    if (prime_len == prime_cap) {
        grow_array((void **)&prime_list, &prime_len, &prime_cap, sizeof(idx_t));
    }
    prime_list[prime_len++] = p;
}

/* Append value to cache array. */
static void add_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        grow_array((void **)&cache, &cache_len, &cache_cap, sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

/* Test if candidate is prime using previously found primes. */
static bool is_prime_candidate(idx_t candidate)
{
    if (candidate < 2) return false;
    if (candidate == 2) return true;
    if ((candidate & 1) == 0) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        idx_t p = prime_list[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n elements in the cache. */
    while (cache_len < n) {
        idx_t candidate = next_candidate++;
        if (!is_prime_candidate(candidate)) {
            continue;
        }
        /* Prime found – store it in the prime list. */
        add_prime(candidate);
        /* Check the quadratic-residue condition. */
        if (isResidue[(int)(candidate % 31)]) {
            add_cache((seq_t)candidate);
        }
    }
    /* Return the nth element (1‑based index). */
    return cache[n - 1];
}
