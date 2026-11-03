/*
 * A080152  Values of n corresponding to the primes given in A073640. The
 *          concatenation of the a(n) and a(n+1)-th primes for any n is also
 *          prime.
 * https://oeis.org/A080152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A080152 – Values of n corresponding to the primes given in A073640. 
 *  The concatenation of the a(n)-th and a(n+1)-th primes is always prime.
 *  a(1) = 1, a(n) is the smallest integer > a(n-1) for which
 *      concat( prime[a(n-1)], prime[a(n)] ) is prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t *cache = NULL; /* a(1), a(2), ... */
static size_t cache_len = 0, cache_cap = 0;

static uint64_t *primes = NULL;  /* 1‑based prime list, stored 0‑based */
static size_t primes_len = 0, primes_cap = 0;
static uint64_t last_prime = 0;

/* initialise static data */
static void init(void)
{
    if (cache != NULL)
        return;                /* already initialised */

    /* sequence cache – start with a(1)=1 */
    cache_cap = 8;
    cache = mem_new(seq_t, cache_cap);
    cache[0] = (seq_t)1;
    cache_len = 1;

    /* prime list – start with 2 */
    primes_cap = 8;
    primes = (uint64_t *)malloc(sizeof(uint64_t) * primes_cap);
    primes[0] = 2;
    primes_len = 1;
    last_prime = 2;
}

/* return a(n) (n >= 1).  For n < 1 returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    init();

    /* generate sequence terms up to index n */
    while (cache_len < (size_t)n) {
        /* current last element of the sequence */
        idx_t last = (idx_t)cache[cache_len - 1];

        /* search for next index that satisfies the concatenation property */
        idx_t j = last + 1;
        for (;; ++j) {
            /* make sure we know the j‑th prime */
            while (primes_len <= (size_t)j) {
                uint64_t p = next_prime(last_prime);
                last_prime = p;

                if (primes_len == primes_cap) {
                    primes_cap *= 2;
                    primes = (uint64_t *)realloc(primes,
                                                sizeof(uint64_t) * primes_cap);
                }
                primes[primes_len++] = p;
            }

            /* concat(prime[last], prime[j]) */
            seq_t concat = concat_parts(primes[last - 1], primes[j - 1]);

            if (is_prime_big(concat)) {
                /* found next element of the sequence */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = (seq_t)j;
                break;
            }
        }
    }

    return cache[n - 1];
}
