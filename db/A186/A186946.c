/*
 * A186946  The smallest integer x > 0 such that the number of prime powers
 *          p^k (k>=1) in (x/2,x] equals n.
 * https://oeis.org/A186946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A186946
 *  a(n) = the smallest x > 0 such that the number of prime powers p^k with k>=1
 *         in the interval (x/2, x] equals n.
 *  This implementation pre‑computes all prime powers up to a fixed limit
 *  (3 000 000) and stores the smallest x giving each count.
 *  It works for any seq_t/idx_t combination that fits in the allowed
 *  integer types or an arbitrary‑precision class in C++.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- constants ------------------------------------------------------- */
#define LIMIT        3000000U          /* maximum x to pre‑compute       */
#define ANSWER_MAX    200000U          /* enough for n up to ~10 000     */
#define PRIME_CAP     250000U          /* enough primes up to LIMIT     */

/* ---- global storage (static, zero‑initialized) --------------------- */
static bool          is_prime[LIMIT + 1];
static bool          is_pp   [LIMIT + 1];
static uint32_t      prefix [LIMIT + 1];
static idx_t         primes  [PRIME_CAP];
static seq_t         answers[ANSWER_MAX + 1];
static bool          initialized = false;

/* ---- helper: pre‑compute primes, prime powers, prefix sums, answers ----- */
static void precompute(void)
{
    /* 1. Sieve of Eratosthenes */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = false;
    is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* 2. Extract primes into array */
    size_t prime_cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (is_prime[p]) {
            if (prime_cnt < PRIME_CAP)
                primes[prime_cnt++] = p;
        }
    }

    /* 3. Mark prime powers */
    for (size_t ii = 0; ii < prime_cnt; ++ii) {
        idx_t p = primes[ii];
        idx_t val = p;
        while (val <= LIMIT) {
            is_pp[val] = true;
            if (val > LIMIT / p) break;
            val *= p;
        }
    }

    /* 4. Prefix sums of prime‑power counts */
    prefix[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i)
        prefix[i] = prefix[i - 1] + (is_pp[i] ? 1U : 0U);

    /* 5. Find smallest x for each count */
    for (idx_t x = 2; x <= LIMIT; ++x) {
        idx_t mid = x / 2;
        uint32_t cnt = prefix[x] - prefix[mid];
        if (cnt <= ANSWER_MAX && answers[cnt] == 0)
            answers[cnt] = (seq_t)x;
    }
}

/* ---- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }
    if (n == 0 || n > ANSWER_MAX) return 0;
    return answers[n];
}
