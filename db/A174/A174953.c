/*
 * A174953  Triangle read by rows: T(n,k) = prime(n) mod semiprime(k),
 *          1<=k<=n.
 * https://oeis.org/A174953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = prime(row(n)) mod semiprime(col(n)) where row(n) is the
 *            row index of the nth term in the triangle and col(n) = n -
 *            row(n)*(row(n)-1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches primes and semiprimes on demand.
   All required arithmetic is performed in the opaque seq_t type
   and the indices use the opaque idx_t type. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Cache structures                                               */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list    = NULL;
static size_t   prime_count    = 0;
static size_t   prime_capacity = 0;

static uint64_t *semiprime_list     = NULL;
static size_t   semiprime_count    = 0;
static size_t   semiprime_capacity = 0;
static uint64_t semiprime_next = 4;   /* next candidate to test */

/* ------------------------------------------------------------------ */
/*  Utility: compute row number for a given linear index n (1‑based)  */
/* ------------------------------------------------------------------ */
static idx_t
row_from_index(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;
    /* Find an upper bound by doubling */
    while (high * (high + 1) / 2 < n) high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Generate primes up to a given limit (inclusive)                    */
/* ------------------------------------------------------------------ */
static void
ensure_primes_up_to(uint64_t limit)
{
    if (prime_count == 0) {
        /* initialise first prime */
        if (prime_capacity == 0) {
            prime_capacity = 16;
            prime_list = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        }
        prime_list[prime_count++] = 2;
    }
    /* Add missing primes */
    uint64_t candidate = prime_list[prime_count - 1] + 1;
    while (candidate <= limit) {
        bool isprime = true;
        uint64_t sqrtc = (uint64_t)floor(sqrt((double)candidate));
        for (size_t i = 0; i < prime_count && prime_list[i] <= sqrtc; ++i) {
            if (candidate % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_count == prime_capacity) {
                prime_capacity = prime_capacity ? prime_capacity * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_capacity * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure we have at least 'needed' primes (1‑based indexing)         */
/* ------------------------------------------------------------------ */
static void
ensure_primes(size_t needed)
{
    if (prime_count >= needed) return;
    /* The n‑th prime is < n (log n + log log n) for n >= 6;
       use a generous estimate to generate enough primes. */
    uint64_t estimate = 0;
    if (needed < 6) estimate = 15;
    else {
        double nd = (double)needed;
        estimate = (uint64_t)(nd * (log(nd) + log(log(nd))) + 10.0);
    }
    ensure_primes_up_to(estimate);
}

/* ------------------------------------------------------------------ */
/*  Generate semiprime numbers (product of exactly two primes)        */
/* ------------------------------------------------------------------ */
static void
ensure_semiprimes(size_t needed)
{
    while (semiprime_count < needed) {
        /* Find next semiprime starting from semiprime_next */
        while (1) {
            uint64_t n = semiprime_next;
            /* Ensure prime list covers sqrt(n) */
            uint64_t sqrtn = (uint64_t)floor(sqrt((double)n));
            ensure_primes_up_to(sqrtn);
            /* Count prime factors with multiplicity */
            uint64_t temp = n;
            int cnt = 0;
            for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
                uint64_t p = prime_list[i];
                while (temp % p == 0) {
                    ++cnt;
                    temp /= p;
                    if (cnt > 2) break;
                }
                if (cnt > 2) break;
            }
            if (temp > 1) ++cnt;
            if (cnt == 2) break; /* found a semiprime */
            ++semiprime_next;
        }
        /* Store the found semiprime */
        if (semiprime_count == semiprime_capacity) {
            semiprime_capacity = semiprime_capacity ? semiprime_capacity * 2 : 16;
            semiprime_list = (uint64_t *)realloc(semiprime_list,
                                                 semiprime_capacity * sizeof(uint64_t));
        }
        semiprime_list[semiprime_count++] = semiprime_next;
        ++semiprime_next;
    }
}

/* ------------------------------------------------------------------ */
/*  The requested sequence function                                  */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0; safe fallback */

    /* Determine row (r) and column (c) from the linear index */
    idx_t r = row_from_index(n);
    idx_t c = n - (r - 1) * r / 2;  /* 1‑based column */

    /* Prepare required primes and semiprimes */
    ensure_primes((size_t)r);
    ensure_semiprimes((size_t)r);

    /* Retrieve the values */
    seq_t prime_val   = (seq_t)prime_list[r - 1];   /* prime(r)   */
    seq_t semiprime_val = (seq_t)semiprime_list[c - 1]; /* semiprime(c) */

    return prime_val % semiprime_val;
}
