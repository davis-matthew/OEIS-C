/*
 * A175156  a(n) = 2^(n-1) mod composite(n).
 * https://oeis.org/A175156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) mod composite(n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (composite(n) is the n-th composite integer, OEIS offset 1) */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* Static storage for primes and composites */
static uint64_t *g_primes = NULL;      /* primes found so far */
static size_t     g_primes_size = 0;   /* number of primes stored */
static size_t     g_primes_cap  = 0;   /* allocated capacity */

static uint64_t *g_composites = NULL;  /* composites found so far */
static size_t     g_comps_size = 0;    /* number of composites stored */
static size_t     g_comps_cap  = 0;    /* allocated capacity */

static uint64_t   g_next = 2;          /* next integer to test (trial) */

/* Ensure at least n composites are generated */
static void ensure_composites(idx_t n)
{
    while (g_comps_size < n) {
        uint64_t x = g_next++;
        /* primality test by trial division using known primes */
        bool is_prime = true;
        uint64_t limit = (uint64_t)sqrt((double)x);
        for (size_t i = 0; i < g_primes_size && g_primes[i] <= limit; ++i) {
            if (x % g_primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {      /* store in prime table */
            if (g_primes_size == g_primes_cap) {
                g_primes_cap = g_primes_cap ? g_primes_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(g_primes,
                          g_primes_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                g_primes = tmp;
            }
            g_primes[g_primes_size++] = x;
        } else {             /* store in composite table */
            if (g_comps_size == g_comps_cap) {
                g_comps_cap = g_comps_cap ? g_comps_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(g_composites,
                          g_comps_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                g_composites = tmp;
            }
            g_composites[g_comps_size++] = x;
        }
    }
}

/* Generic modular exponentiation using seq_t arithmetic.
   (seq_t::operator* and operator% are defined for the arbitrary‑precision type.) */
static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) {
            result = (seq_t)((seq_t)result * base % mod);
        }
        base = (seq_t)((seq_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, harmless value */
    ensure_composites(n);
    seq_t mod = (seq_t)g_composites[n - 1];
    return mod_pow((seq_t)2, n - 1, mod);
}
