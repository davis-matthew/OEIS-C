/*
 * A058340  Primes p such that phi(x) = p-1 has only 2 solutions, namely x = p
 *          and x = 2p.
 * https://oeis.org/A058340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 4552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058340
   Primes p such that the equation φ(x) = p-1 has only the solutions x = p and x = 2p.
   The function searches primes until the nth such prime is found. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data --------------------------------------------------------- */
static uint64_t *prime_pool    = NULL;   /* array of primes                */
static size_t    prime_cnt     = 0;      /* number of primes generated     */
static size_t    prime_cap     = 0;      /* allocated capacity of prime_pool */

static seq_t   *seq_storage   = NULL;   /* dynamic array of sequence terms*/
static size_t   seq_len       = 0;       /* number of terms stored         */
static size_t   seq_cap       = 0;       /* allocated capacity of seq_storage */

static idx_t    last_prime_idx = 0;   /* index of next prime to test      */

/* -------------------------------------------------------------------- */
/* Create a list of primes up to a fixed limit.  The limit is chosen
   conservatively so that the first 10 000 terms of the sequence are
   guaranteed to be found.                                                     */
static void init_primes(void)
{
    if (prime_cnt) return;   /* already initialized */

    /* 2 million is more than enough for the first many terms.          */
    const size_t LIMIT = 2000000;

    char *sieve = (char*)calloc(LIMIT + 1, 1);
    if (!sieve) exit(1);

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;

    /* Estimate the number of primes (~LIMIT / log L). */
    prime_cap = LIMIT / 10;          /* initial guess */
    prime_pool = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_pool) exit(1);

    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) {
            if (prime_cnt >= prime_cap) {            /* grow array */
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_pool, prime_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_pool = tmp;
            }
            prime_pool[prime_cnt++] = i;
        }

    free(sieve);
}

/* -------------------------------------------------------------------- */
/* Append a value to the dynamic array `seq_storage`.                    */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? (seq_cap << 1) : 4;
        seq_storage = (seq_t*)realloc(seq_storage, seq_cap * sizeof(seq_t));
        if (!seq_storage) exit(1);
    }
    seq_storage[seq_len++] = val;
}

/* -------------------------------------------------------------------- */
/* Recursive depth‑first search to check whether there exists a
   non‑trivial solution of φ(x) = p-1.  `rem` is the remaining value
   of φ(x); `start` is the index in `prime_pool` from which we may
   choose further odd primes; `count` counts how many distinct odd
   primes have been used so far; `has_p` indicates whether the prime
   `p` itself has already appeared as a factor.                     */
static bool dfs_rec(uint64_t rem, size_t start, int count, bool has_p, uint64_t p)
{
    if (rem == 1) {
        /* rem reached 1 with exactly one odd prime factor that equals p
           (case x = p) or no odd primes (case x = 2p). */
        if (count == 1 && has_p)    /* x = p */
            return false;
        return true;                /* other cases available */
    }

    for (size_t i = start; i < prime_cnt; ++i) {
        uint64_t q = prime_pool[i];
        if (q <= 2) continue;        /* skip even prime 2 */

        uint64_t q_minus = q - 1;
        if (q_minus > rem) break;    /* remaining primes too large   */
        if (rem % q_minus) continue; /* q−1 must divide rem          */

        /* try all exponents e ≥ 1 for the prime q */
        uint64_t pow_q = 1;          /* q^(e−1) */
        for (int e = 1; ; ++e) {
            uint64_t phi_q = pow_q * q_minus;   /* φ(q^e) = q^(e−1)(q−1) */
            if (phi_q > rem) break;
            if (rem % phi_q == 0) {
                if (dfs_rec(rem / phi_q, i + 1, count + 1,
                            has_p || (q == p), p))
                    return true;
            }
            /* prevent overflow on next multiplication */
            if (pow_q > UINT64_MAX / q) break;
            pow_q *= q;
        }
    }
    return false;
}

/* -------------------------------------------------------------------- */
/* Check whether the prime `p` has a non‑trivial solution of
   φ(x) = p-1.  If such a solution exists the function returns true,
   otherwise false.                                                     */
static bool has_other_solution(uint64_t p)
{
    uint64_t n = p - 1;

    /* Determine v₂(p-1) (the 2‑adic valuation). */
    int a2 = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0ULL) { tmp >>= 1; ++a2; }

    /* Try all possible powers of 2 that could appear in x.                */
    for (int k = 0; k <= a2 + 1; ++k) {
        uint64_t rem = (k == 0) ? n : (n >> (k - 1));
        if (dfs_rec(rem, 0, 0, false, p))
            return true;                /* extra solution found */
    }
    return false;                       /* only the two expected solutions */
}

/* -------------------------------------------------------------------- */
/* The required OEIS function.                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the OEIS; safe to return 0 */

    init_primes();                 /* ensure prime list is ready */

    /* Keep generating primes until we have `n` valid terms. */
    while (seq_len < n) {
        if (last_prime_idx >= prime_cnt) break;  /* safety guard */

        uint64_t candidate = prime_pool[last_prime_idx++];
        if (candidate < 3) continue;             /* 2 is not in the sequence */

        if (!has_other_solution(candidate))
            seq_push((seq_t)candidate);
    }

    return seq_storage[n - 1];
}
