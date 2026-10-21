/*
 * A065047  Primes which when written in base 2 and prepended with a 1 produce
 *          a prime.
 * https://oeis.org/A065047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = the n‑th prime p such that * p and (2^k + p) are prime,
 *            where * k = ⌊log₂ p⌋ + 1 (i.e. the smallest integer with 2^k >
 *            p) * * Method: 1) Generate primes on demand with trial division.
 *            * 2) For each prime p, compute k (number of bits needed for p) *
 *            and q = 2^k + p. Test q for primality usi...
 * Range:     u8 n <= 14, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* static storage                                                 */
/* ------------------------------------------------------------------ */
#define MAX_PRIMES 40000   /* enough for generating primes up to ~5 million */
#define MAX_SEQ   20000    /* enough for n up to 10 000 */

static uint64_t primes[MAX_PRIMES];
static idx_t    prime_cnt   = 0;
static uint64_t last_checked = 1;

static seq_t seq_terms[MAX_SEQ];
static idx_t seq_len = 0;

/* ------------------------------------------------------------------ */
/* helper: extend prime list up to N (inclusive)                       */
/* ------------------------------------------------------------------ */
static void ensure_primes_upto(uint64_t N)
{
    if (N <= last_checked) return;

    if (prime_cnt == 0) {                     /* initialise with 2 */
        primes[prime_cnt++] = 2;
        last_checked = 2;
        if (N <= 2) return;
    }

    for (uint64_t cand = last_checked + 1; cand <= N; ++cand) {
        bool is_prime = true;
        uint64_t lim = (uint64_t)floor(sqrt((long double)cand));
        for (idx_t i = 0; i < prime_cnt && primes[i] <= lim; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt >= MAX_PRIMES)
                exit(1);                /* too many primes needed */
            primes[prime_cnt++] = cand;
        }
    }
    last_checked = N;
}

/* ------------------------------------------------------------------ */
/* primality test for q expressed as unsigned __int128                 */
/* ------------------------------------------------------------------ */
static bool is_prime_uint128(unsigned __int128 q)
{
    if (q < 2) return false;

    uint64_t limit = (uint64_t)floor(sqrt((long double)q));
    while ((unsigned __int128)limit * limit > q) --limit;
    while ((unsigned __int128)(limit + 1) * (limit + 1) <= q) ++limit;

    ensure_primes_upto(limit);
    for (idx_t i = 0; i < prime_cnt && primes[i] <= limit; ++i)
        if (q % primes[i] == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* build sequence until it has at least n terms                         */
/* ------------------------------------------------------------------ */
static void ensure_sequence(idx_t n)
{
    static idx_t last_primes_used = 0;       /* where we left off in primes[] */

    while (seq_len < n) {
        /* generate more primes if needed */
        while (last_primes_used >= prime_cnt) {
            uint64_t new_limit = last_checked + 1000;
            ensure_primes_upto(new_limit);
        }

        uint64_t p = primes[last_primes_used++];
        /* compute k = number of bits of p (so 2^k > p) */
        unsigned k = 0;
        uint64_t tmp = p;
        while (tmp) { ++k; tmp >>= 1; }

        unsigned __int128 q = ((unsigned __int128)1 << k) + p;
        if (is_prime_uint128(q)) {
            if (seq_len >= MAX_SEQ) exit(1); /* sequence too long for our buffer */
            seq_terms[seq_len++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* required public function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1: a(1) is first term */
    ensure_sequence(n);
    return seq_terms[n - 1];
}
