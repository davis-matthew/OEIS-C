/*
 * A141282  Least prime p such that p-6^n, ..., p-6, p, p+6, ... and p+6^n are
 *          primes.
 * https://oeis.org/A141282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least prime p such that p-6^k … p+6^k are all prime
 * for every integer k with 1 ≤ k ≤ n. The pattern is impossible for n ≥ 5,
 * so a(n) = 0 for n > 4.
 *
 * The routine computes the value for any n ≥ 1 (memoised for repeated calls)
 * within the time limits by generating a small sieve once and testing each
 * prime candidate up to the known upper bound for n = 4.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit,
                                           uint64_t **primes_out,
                                           size_t *cnt_out);            */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t x);          */

seq_t a(idx_t n)
{
    /* Return 0 for n = 0 (outside offset) or n > 4 (no such prime). */
    if (n == 0 || n > 4)
        return (seq_t)0;

    /* Memoisation cache for values 1..4.  Index 0 unused. */
    static seq_t cache[5] = {0};
    if (cache[n] != 0)
        return cache[n];

    /* Build a small sieve once.  20 000 000 covers all primes needed for n ≤ 4. */
    static bool sieve_done = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    if (!sieve_done) {
        const idx_t SIEVE_LIMIT = 20000000ULL;
        build_primes_upto(SIEVE_LIMIT, &primes, &prime_cnt);
        sieve_done = true;
    }

    /* Pre‑compute powers of 6 up to 4. */
    static idx_t pow6[5];
    static bool pow6_done = false;
    if (!pow6_done) {
        pow6[0] = 1;
        for (idx_t i = 1; i <= 4; ++i)
            pow6[i] = pow6[i - 1] * 6;
        pow6_done = true;
    }

    /* Test each candidate prime from the sieve. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t cand = primes[i];

        /* Candidate must be large enough to allow all subtractions. */
        if (cand < pow6[n] + 2)
            continue;

        bool ok = true;
        for (idx_t k = 1; k <= n && ok; ++k) {
            uint64_t d = pow6[k];
            if (cand < d + 2) {  /* would produce a number < 2 */
                ok = false;
                break;
            }
            if (!is_prime(cand - d) || !is_prime(cand + d))
                ok = false;
        }

        if (ok) {
            cache[n] = (seq_t)cand;
            return cache[n];
        }
    }

    /* Should never reach here for n ≤ 4, but return 0 as a safe fallback. */
    cache[n] = (seq_t)0;
    return cache[n];
}
