/*
 * A106085  Primes p such that 5*p + 22 and 22*p + 5 are primes.
 * https://oeis.org/A106085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A106085:  Primes p such that 5*p+22 and 22*p+5 are primes.  */
/*  The function generates primes incrementally and stores those that
    satisfy the condition.  It uses static arrays sized sufficiently
    for n up to at least 10000.  No dynamic allocation is performed,
    so it works for any seq_t, including large‑integer types.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* maximum numbers of primes we will keep: enough for n <= 10000 */
#define MAX_PRIMES 200000          /* 2e5 > expected number of primes needed */

/* ---------------  static tables  ----------------- */
static uint64_t prime_list[MAX_PRIMES];
static size_t   prime_count   = 0;
static uint64_t next_candidate = 2U;   /* next odd candidate to test */

static uint64_t valid_primes[MAX_PRIMES];
static size_t   valid_count   = 0;

/* ---------------  helper  ----------------- */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1U) == 0) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* make sure we have at least n valid primes */
    while (valid_count < (size_t)n) {
        /* find next prime */
        uint64_t p;
        for (;;) {
            /* skip even numbers beyond 2 */
            if (next_candidate <= 2U) {
                next_candidate = 2U;
            } else if (next_candidate == 2U) {
                next_candidate = 3U;
            } else if ((next_candidate & 1U) == 0) {
                ++next_candidate;
                continue;
            }

            bool divisible = false;
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t pr = prime_list[i];
                if (pr > next_candidate / pr) break;
                if (next_candidate % pr == 0) {
                    divisible = true;
                    break;
                }
            }
            if (!divisible) {          /* prime found */
                p = next_candidate;
                if (prime_count < MAX_PRIMES)
                    prime_list[prime_count++] = p;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }

        /* check the required conditions */
        uint64_t x = 5U * p + 22U;
        uint64_t y = 22U * p + 5U;
        if (is_prime_uint64(x) && is_prime_uint64(y)) {
            if (valid_count < MAX_PRIMES)
                valid_primes[valid_count++] = p;
        }
    }
    /* return the n‑th valid prime, converted to seq_t */
    return (seq_t)valid_primes[(size_t)n - 1];
}
