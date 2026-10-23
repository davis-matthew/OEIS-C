/*
 * A147624  Numbers n = concat(a,b) such that omega(n) = omega(a) * omega(b),
 *          where omega = A001221.
 * https://oeis.org/A147624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n‑th integer that can be written as concat(a,b) such
 *            that omega(concat(a,b)) = omega(a) * omega(b). (omega is
 *            A001221, the number of distinct prime factors.)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes required for the helpers and standard types */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"

/* ------------------------------------------------------------ */
/*  Auxiliary data and functions                                 */
/* ------------------------------------------------------------ */

/* pre‑computed powers of 10 up to 10^19 (fits in uint64_t)      */
static const uint64_t pow10[20] = {
    1ULL,                      /* 10^0 */
    10ULL,                     /* 10^1 */
    100ULL,                    /* 10^2 */
    1000ULL,                   /* 10^3 */
    10000ULL,                  /* 10^4 */
    100000ULL,                 /* 10^5 */
    1000000ULL,                /* 10^6 */
    10000000ULL,               /* 10^7 */
    100000000ULL,              /* 10^8 */
    1000000000ULL,             /* 10^9 */
    10000000000ULL,            /* 10^10 */
    100000000000ULL,           /* 10^11 */
    1000000000000ULL,          /* 10^12 */
    10000000000000ULL,         /* 10^13 */
    100000000000000ULL,        /* 10^14 */
    1000000000000000ULL,       /* 10^15 */
    10000000000000000ULL,      /* 10^16 */
    100000000000000000ULL,     /* 10^17 */
    1000000000000000000ULL,    /* 10^18 */
    10000000000000000000ULL    /* 10^19 */
};

/* number of decimal digits of a 64‑bit unsigned integer */
static inline unsigned digit_len_uint64(uint64_t x)
{
    unsigned len = 1;
    while (x >= 10ULL) {
        x /= 10ULL;
        ++len;
    }
    return len;
}

/* ------------------------------------------------------------ */
/*  Cache for the sequence                                       */
/* ------------------------------------------------------------ */

static seq_t *seq_cache      = NULL;   /* array that holds the terms          */
static size_t seq_cache_size = 0;      /* number of terms currently stored    */
static size_t seq_cache_cap  = 0;      /* allocated capacity                   */
static uint64_t last_checked = 1;      /* the last integer already examined   */

/* ensure the cache can store at least one more element */
static void ensure_cache_capacity(void)
{
    if (seq_cache_size >= seq_cache_cap) {
        size_t new_cap = seq_cache_cap == 0 ? 64 : seq_cache_cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (new_ptr) {
            seq_cache = new_ptr;
            seq_cache_cap = new_cap;
        }
    }
}

/* generate terms until the cache contains at least 'target' entries */
static void generate_until(size_t target)
{
    while (seq_cache_size < target) {
        uint64_t cand = ++last_checked;         /* candidate integer   */

        unsigned d = digit_len_uint64(cand);     /* number of decimal digits */
        if (d < 2) continue;                    /* need at least two digits for a split */

        uint64_t omega_c = (uint64_t)count_distinct_primes(cand);

        /* try every possible split position */
        for (unsigned k = 1; k < d; ++k) {
            unsigned len_b = d - k;                   /* digits in b       */
            uint64_t div = pow10[len_b];              /* 10^len_b          */

            uint64_t a = cand / div;
            uint64_t b = cand % div;

            /* b must have exactly len_b digits (no leading zero) */
            if (b < pow10[len_b - 1]) continue;

            uint64_t omega_a = (uint64_t)count_distinct_primes(a);
            uint64_t omega_b = (uint64_t)count_distinct_primes(b);

            if (omega_c == omega_a * omega_b) {
                /* found a term */
                ensure_cache_capacity();
                seq_cache[seq_cache_size++] = (seq_t)cand;
                break;  /* move on to the next candidate integer */
            }
        }
    }
}

/* ------------------------------------------------------------ */
/*  Main public function                                         */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, return 0 */

    size_t target = (size_t)n;    /* recursion safely in size_t range   */
    generate_until(target);
    return seq_cache[n - 1];
}
