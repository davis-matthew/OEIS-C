/*
 * A205522  Primes resulting from adding x and y from the least positive
 *          solution to Pell's equation (x^2 - d*y^2 == 1), with d squarefree.
 * https://oeis.org/A205522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 34, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_isqrt_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes resulting from adding the fundamental x+y of Pell’s equation
   x^2 - d*y^2 = 1 for squarefree d. The sequence terms are generated
   in increasing order of d. */

/* --------------------------------------------------------------- */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64(idx_t)   */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t)      */

/* ---------- local helpers ------------------------------------- */

/* Sieve of Eratosthenes: generates all primes up to LIMIT.
   Primes are stored in the static array "primes" and their count
   in "primes_cnt".  The sieve is performed once on first use. */
#define SIEVE_LIMIT   1000000

static uint64_t primes[SIEVE_LIMIT / 10 + 1000];
static size_t primes_cnt = 0;
static bool primes_built  = false;

static void build_primes(void) {
    if (primes_built) return;
    size_t limit = SIEVE_LIMIT;
    bool *comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!comp) return;   /* allocation failure – unlikely */
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (primes_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
    primes_built = true;
}

/* Check whether n is squarefree.
   n is assumed to be >1. */
static bool is_squarefree(uint64_t n) {
    if (n <= 1) return false;   /* 1 is not squarefree for our purposes */
    uint64_t lim = floor_isqrt_u64(n);
    for (size_t i = 0; i < primes_cnt && primes[i] <= lim; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (p2 > n) break;
        if (n % p2 == 0) return false;
    }
    return true;
}

/* Compute the sum x + y for the fundamental solution of
   x^2 - d*y^2 = 1.  The result is returned as seq_t.  For d
   that are not squarefree or are perfect squares, the function
   returns 0.  The implementation uses the standard continued
   fraction method. */
static seq_t pell_sum(uint64_t d) {
    if (!is_squarefree(d)) return (seq_t)0;
    uint64_t a0 = floor_isqrt_u64(d);
    if (a0 * a0 == d) return (seq_t)0;      /* d is a perfect square */

    /* Compute continued fraction expansion of sqrt(d). */
    uint64_t m = 0;
    uint64_t dd = 1;
    uint64_t a = a0;
    size_t period_len = 0;
    size_t period_cap = 16;
    uint64_t *period = (uint64_t*)malloc(period_cap * sizeof(uint64_t));
    if (!period) return (seq_t)0;   /* allocation failure */

    while (true) {
        m  = dd * a - m;
        dd = (d - m * m) / dd;
        a  = (a0 + m) / dd;          /* integer division */

        if (period_len == period_cap) {
            period_cap <<= 1;
            uint64_t *tmp = (uint64_t*)realloc(period, period_cap * sizeof(uint64_t));
            if (!tmp) { free(period); return (seq_t)0; }
            period = tmp;
        }
        period[period_len++] = a;

        if (a == 2 * a0) break;   /* period ended */
    }

    /* Determine the index of the convergent that gives the
       fundamental solution.  The period length decides which
       convergent it is. */
    size_t target_index;
    if (period_len % 2 == 0)
        target_index = period_len - 1;
    else
        target_index = 2 * period_len - 1;

    /* Compute the convergents up to target_index. */
    seq_t p_prevprev = (seq_t)0, p_prev = (seq_t)1;
    seq_t q_prevprev = (seq_t)1, q_prev = (seq_t)0;
    seq_t p_cur, q_cur;
    uint64_t a_term;

    for (size_t k = 0; k <= target_index; ++k) {
        if (k == 0)
            a_term = a0;
        else
            a_term = period[(k - 1) % period_len];

        p_cur = ((seq_t)a_term) * p_prev + p_prevprev;
        q_cur = ((seq_t)a_term) * q_prev + q_prevprev;

        p_prevprev = p_prev;  p_prev = p_cur;
        q_prevprev = q_prev;  q_prev = q_cur;
    }

    free(period);
    return p_cur + q_cur;   /* x + y */
}

/* ---------- main sequence function ------------------------------ */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_d = 2;

/* seq_t a(idx_t n)
   Returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* Ensure primes are generated once. */
    build_primes();

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        uint64_t d = next_d++;
        seq_t sum  = pell_sum(d);
        if (sum != (seq_t)0 && is_prime_big(sum)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;    /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = sum;
        }
    }
    return cache[n - 1];
}
