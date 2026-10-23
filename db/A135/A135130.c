/*
 * A135130  Let n = Product_{p} p ^ e_p be the prime factorization of n and
 *          let M = max{e_p + 1 }. Then n is in the sequence iff for all
 *          primes q in the range 3 <= q <= M we have e_q >= Sum_{r} floor(
 *          log_q (e_r + 1) ).
 * https://oeis.org/A135130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer n such that for all primes q
 *            with 3 ≤ q ≤ M = max{ e_p } + 1, the exponent of q in n is at
 *            least the sum over all primes r dividing n of floor(log_q(e_r +
 *            1)). This is the definition of OEIS sequence A135130. The
 *            following implementation generates the sequence by...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Global data for prime table */
/* --------------------------------------------------------------------- */
static bool   primes_built     = false;
static idx_t *prime_list       = NULL;      /* array of primes up to LIMIT */
static size_t prime_cnt        = 0;          /* number of primes in prime_list */

static void build_prime_table(void)
{
    if (primes_built) return;

    /* Sieve up to 1,000,000 – enough for all candidates expected in this
       sequence for n up to at least 10,000. */
    const idx_t LIMIT = 1000000UL;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) { abort(); }

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* Count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++prime_cnt;

    /* Allocate and store primes */
    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!prime_list) { abort(); }

    size_t pidx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[pidx++] = i;

    free(is_prime);
    primes_built = true;
}

/* --------------------------------------------------------------------- */
/* Check whether a number satisfies the OEIS condition */
/* --------------------------------------------------------------------- */
static bool is_valid(idx_t n)
{
    if (n == 1) return true;            /* 1 is always included */

    /* Factorise n using the precomputed prime list.  We only need a few
       exponents, so a small fixed array suffices. */
    idx_t  primes_local[32];
    uint8_t exps_local[32];
    size_t pcnt = 0;

    idx_t tmp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_local[pcnt]   = p;
            exps_local[pcnt++]   = e;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        primes_local[pcnt]   = tmp;
        exps_local[pcnt++]   = 1;
    }

    /* Determine M = max(e_p) + 1 */
    uint8_t M = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint8_t val = exps_local[i] + 1;
        if (val > M) M = val;
    }
    if (M < 3) return true;           /* no primes q to check */

    /* For each prime q ≤ M (q ≥ 3) check the condition */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint8_t q = (uint8_t)prime_list[i];
        if (q > M) break;
        if (q < 3) continue;          /* skip q = 2 */

        /* Compute the sum over primes r of floor(log_q(e_r + 1)) */
        int needed = 0;
        for (size_t j = 0; j < pcnt; ++j) {
            uint8_t k = exps_local[j] + 1;
            int t = 0;
            while (k >= q) {
                k /= q;
                ++t;
            }
            needed += t;
        }

        /* Find e_q from the factorization, if present */
        int eq = 0;
        for (size_t j = 0; j < pcnt; ++j) {
            if (primes_local[j] == q) {
                eq = exps_local[j];
                break;
            }
        }

        if (eq < needed) return false;
    }

    return true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;  /* array of computed terms */
    static size_t term_count = 0;     /* number of terms stored */
    static idx_t cur_candidate = 1;   /* next candidate to test */

    /* Prepare prime table once */
    if (!primes_built) build_prime_table();

    /* Generate terms up to index n (1‑based) */
    while (term_count < n) {
        if (is_valid(cur_candidate)) {
            /* Grow array if necessary */
            seq_t *new_terms = (seq_t *)realloc(terms, (term_count + 1) * sizeof(seq_t));
            if (!new_terms) { abort(); }
            terms = new_terms;
            terms[term_count++] = (seq_t)cur_candidate;
        }
        ++cur_candidate;
    }

    return terms[n - 1];
}
