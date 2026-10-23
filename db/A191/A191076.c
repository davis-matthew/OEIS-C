/*
 * A191076  Primes p that have Kronecker symbol (p|62) = -1.
 * https://oeis.org/A191076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p with Kronecker symbol (p|62) = -1 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Legendre residue table modulo 31.  true if r is a quadratic residue */
static const bool residue31[31] = {
    false, /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    false, /* 3 */
    true,  /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    true,  /* 8 */
    true,  /* 9 */
    true,  /*10 */
    false, /*11 */
    false, /*12 */
    false, /*13 */
    true,  /*14 */
    false, /*15 */
    true,  /*16 */
    false, /*17 */
    true,  /*18 */
    true,  /*19 */
    true,  /*20 */
    false, /*21 */
    false, /*22 */
    false, /*23 */
    false, /*24 */
    true,  /*25 */
    false, /*26 */
    false, /*27 */
    true,  /*28 */
    false, /*29 */
    false  /*30 */
};

/* Dynamic array of primes */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

static void ensure_cap_primes(size_t need)
{
    if (need > primes_cap) {
        size_t newcap = primes_cap ? primes_cap * 2 : 256;
        if (newcap < need) newcap = need;
        uint64_t *p = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        primes = p;
        primes_cap = newcap;
    }
}

/* Dynamic array of qualifying primes (sequence values) */
static seq_t *filtered = NULL;
static size_t filtered_cnt = 0, filtered_cap = 0;

static void ensure_cap_filtered(size_t need)
{
    if (need > filtered_cap) {
        size_t newcap = filtered_cap ? filtered_cap * 2 : 16;
        if (newcap < need) newcap = need;
        seq_t *p = (seq_t *)realloc(filtered, newcap * sizeof(seq_t));
        filtered = p;
        filtered_cap = newcap;
    }
}

/* Integer primality test using trial division by stored primes */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = (uint64_t)sqrt((double)n);
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

static uint64_t next_candidate = 3;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Initialise prime list with first prime 2 */
        ensure_cap_primes(1);
        primes[primes_cnt++] = 2;
        /* Initialise filtered list */
        ensure_cap_filtered(16);
        filtered_cnt = 0;
        initialized = true;
    }

    while (filtered_cnt < (size_t)n) {
        /* Test current candidate for primality */
        if (is_prime(next_candidate)) {
            /* Store prime for future tests */
            ensure_cap_primes(primes_cnt + 1);
            primes[primes_cnt++] = next_candidate;

            /* Skip the prime 31, its Kronecker symbol is 0 */
            if (next_candidate != 31) {
                /* Compute Jacobi symbol (p|62) = (p|2)*(p|31) */
                int jacobi2 = ((next_candidate & 7) == 1 || (next_candidate & 7) == 7) ? 1 : -1;
                int jacobi31 = residue31[next_candidate % 31] ? 1 : -1;
                if (jacobi2 * jacobi31 == -1) {
                    /* qualifies for the sequence */
                    ensure_cap_filtered(filtered_cnt + 1);
                    filtered[filtered_cnt++] = (seq_t)next_candidate;
                }
            }
        }
        /* Move to next odd number */
        next_candidate += 2;
    }

    return filtered[n - 1];
}
