/*
 * A228377  Primes of the form XYYYYYYYX, where Y is a single digit.
 * https://oeis.org/A228377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   digit_sum, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A228377: primes of the form XYYYYYYYX, where Y is a single digit. 
 * The function generates the numbers in ascending order and keeps a cache 
 * of primes found so far.  It can be called in any order of n and will 
 * generate additional terms on demand.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/pow10.h"          /* seq_t pow10(idx_t) */
#include "../utils/digit_sum.h"      /* seq_t digit_sum(seq_t) */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t) */

/* Powers of ten up to 10^19 for 64‑bit integer limits */
static const idx_t POW10_INT[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Prime cache */
static seq_t *prime_cache   = NULL;
static size_t prime_len     = 0;
static size_t prime_cap     = 0;

/* Generation state */
static bool initialized      = false;
static idx_t  gen_k          = 1;    /* current number of digits of X   */
static idx_t  gen_A          = 1;    /* current value of X              */
static int    gen_Y          = 0;    /* current Y (0‑9)                 */

static seq_t pow10_7_cache;         /* 10^7                          */
static seq_t unit_cache;            /* 1111111 ( = (10^7–1)/9 )     */
static seq_t pow10_k_cache;         /* 10^k                           */
static seq_t term_base_cache;       /* 10^7·10^k + 1                 */

/* Ensure prime cache has room for at least 'need' elements */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap : 64;
    while (new_cap < need) new_cap <<= 1;
    prime_cache = (seq_t*)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cap = new_cap;
}

/* Advance to the next candidate (X, Y) pair */
static void advance_candidate(void)
{
    gen_Y++;
    if (gen_Y == 10) {
        gen_Y = 0;
        gen_A++;
        idx_t upper_A = POW10_INT[gen_k] - 1;
        if (gen_A > upper_A) {
            /* Move to next length of X */
            gen_k++;
            gen_A = POW10_INT[gen_k - 1];
            /* Re‑compute k‑dependent constants */
            pow10_k_cache   = pow10(gen_k);
            term_base_cache = pow10_7_cache * pow10_k_cache + 1;
        }
    }
}

/* Generate primes until we have at least 'limit' primes in the cache */
static void generate_primes_up_to(idx_t limit)
{
    while (prime_len < limit) {
        /* Filter out obvious non‑primes by last digit of X */
        idx_t last_digit = gen_A % 10;
        if (last_digit == 1 || last_digit == 3 ||
            last_digit == 7 || last_digit == 9) {

            /* Build candidate N = X*(10^7·10^k + 1) + Y*1111111*10^k */
            seq_t X_seq = (seq_t)gen_A;
            seq_t B     = (seq_t)gen_Y * unit_cache;
            seq_t N     = X_seq * term_base_cache + B * pow10_k_cache;

            /* Quick sieve: not divisible by 3 */
            if ((digit_sum(N) % 3) != 0 && isPrimeGeneric(N)) {
                ensure_prime_cap(prime_len + 1);
                prime_cache[prime_len++] = N;
            }
        }
        advance_candidate();
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n=0, but return 0 */

    if (!initialized) {
        /* First call: initialise constants and allocate cache */
        pow10_7_cache = pow10(7);
        unit_cache    = (pow10_7_cache - 1) / (seq_t)9;   /* 1111111 */
        pow10_k_cache = pow10(gen_k);                     /* 10^1    */
        term_base_cache = pow10_7_cache * pow10_k_cache + 1; /* 10^(k+7)+1 */

        prime_cache = (seq_t*)malloc(sizeof(seq_t) * 64);
        prime_cap   = 64;
        prime_len   = 0;
        initialized = true;
    }

    if ((idx_t)n > prime_len) {
        generate_primes_up_to(n);
    }

    return prime_cache[n - 1];
}
