/*
 * A074243  Numbers n such that every integer has a cube root mod n.
 * https://oeis.org/A074243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): nth number such that the cube map x^3 (mod n) is a bijection.
   Sequence consists of all squarefree numbers whose prime divisors are
   either 3 or congruent to 2 modulo 3. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint32_t *g_spf = NULL;          /* smallest prime factor */
static uint32_t *g_primes = NULL;       /* list of primes up to current limit */
static size_t   g_prime_cnt = 0;

/* Build the smallest‑prime‑factor sieve up to 'limit'. */
static void build_spf(uint32_t limit)
{
    free(g_spf);
    g_spf = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    /* scratch space for primes */
    free(g_primes);
    size_t primes_cap = limit / 10 + 10;
    g_primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    g_prime_cnt = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = i;
            if (g_prime_cnt >= primes_cap) {
                primes_cap <<= 1;
                g_primes = (uint32_t*)realloc(g_primes,
                                            primes_cap * sizeof(uint32_t));
            }
            g_primes[g_prime_cnt++] = i;
        }
        for (size_t j = 0; j < g_prime_cnt &&
                          g_primes[j] <= g_spf[i] &&
                          (uint64_t)i * g_primes[j] <= limit; ++j) {
            g_spf[i * g_primes[j]] = g_primes[j];
        }
    }
}

/* Return true iff all prime factors of 'm' are 3 or ≡2 (mod 3) and m is squarefree. */
static bool is_good(uint32_t m)
{
    if (m == 1) return true;
    uint32_t last_p = 0;
    while (m > 1) {
        uint32_t p = g_spf[m];
        if (p == 0) p = m;          /* safety for very large primes */
        if (p == last_p) return false;           /* square factor */
        if (p != 3 && (p % 3u) != 2u) return false;
        last_p = p;
        m /= p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe */

    /* static storage for the generated sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint32_t final_limit = 0;    /* current sieve limit */
    static uint32_t next_to_check = 2;  /* next integer to test */

    if (!seq) {                         /* first call */
        seq_cap = 128;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 1;
        seq_len = 1;
    }

    while (seq_len < (size_t)n) {
        /* enlarge sieve if necessary */
        if (final_limit < next_to_check) {
            uint32_t old_limit = final_limit;
            final_limit = (final_limit == 0 ? 100 : final_limit * 2);
            build_spf(final_limit);
            next_to_check = old_limit + 1;
            if (next_to_check < 2) next_to_check = 2;
        }

        /* scan numbers up to the current sieve limit */
        for (; next_to_check <= final_limit && seq_len < (size_t)n;
             ++next_to_check) {
            if (is_good(next_to_check)) {
                if (seq_len >= seq_cap) {
                    seq_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(seq, seq_cap *
                                                sizeof(seq_t));
                    if (!tmp) return 0;            /* allocation failure */
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)next_to_check;
            }
        }
    }

    return seq[n - 1];
}
