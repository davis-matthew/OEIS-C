/*
 * A076546  Let P = { p_1 = 3, p_2 = 5, ...} be the set of odd primes. If p_n
 *          in P can be written as p_n = q+r+s with q, r, s in P, let a(n) =
 *          largest such q, otherwise let a(n) = p_{n+1}.
 * https://oeis.org/A076546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS sequence A076546.
 *
 * Let P = { p_1 = 3, p_2 = 5, ... } be the set of odd primes.
 * For n ≥ 1, let p_n be the n-th odd prime.  If
 *   p_n = q + r + s (with q, r, s ∈ P)  then
 *   a(n) = the largest such q,
 * otherwise
 *   a(n) = p_{n+1}.
 *
 * The routine precalculates enough odd primes to cover n up to about
 * 10 000 and then evaluates the rule for each prime.  A simple sieve
 * and an array of pre‑computed Goldbach facts are used.  The
 * implementation is compliant with both C11 and C++17.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations of helper functions */
static void init_sieve(void);
static bool has_gold_breakdown(idx_t even);
static seq_t next_odd_prime(seq_t p);

/* Global static state */
static bool initialized = false;
static const idx_t SIEVE_LIMIT = 400000;       /* enough primes for n ≈ 10000 */

static unsigned char *is_prime = NULL;          /* 0/1 flags */
static unsigned *primes = NULL;                /* list of odd primes */
static size_t primes_cnt = 0;                  /* number of odd primes */

static signed char *gold_ex = NULL;            /* -1 = unknown, 0 = no, 1 = yes */
static seq_t *seq_cache = NULL;                /* a(n) for each odd prime index */

/* ------------------------------------------------------------------ */
/* Sieve and tables -------------------------------------------------- */

static void init_sieve(void)
{
    /* Allocate and run sieve */
    size_t lim = SIEVE_LIMIT;
    is_prime = (unsigned char*)malloc((lim + 1) * sizeof(unsigned char));
    for (size_t i = 0; i <= lim; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= lim; j += i) is_prime[j] = 0;
        }
    }

    /* Count odd primes and build list */
    for (size_t i = 3; i <= lim; i += 2)
        if (is_prime[i]) ++primes_cnt;
    primes = (unsigned*)malloc(primes_cnt * sizeof(unsigned));
    size_t idx = 0;
    for (size_t i = 3; i <= lim; i += 2)
        if (is_prime[i]) primes[idx++] = (unsigned)i;

    /* Goldbach existence helper array */
    gold_ex = (signed char*)malloc((lim + 1) * sizeof(signed char));
    for (size_t i = 0; i <= lim; ++i) gold_ex[i] = -1;
}

/* ------------------------------------------------------------------ */
/* Goldbach helper --------------------------------------------------- */

static bool has_gold_breakdown(idx_t even)
{
    if (even < 6 || (even & 1)) return false;

    size_t e = (size_t)even;
    if (gold_ex[e] != -1) return gold_ex[e] == 1;

    /* Check for a pair of odd primes summing to 'even' */
    for (size_t i = 0; i < primes_cnt; ++i) {
        size_t r = (size_t)primes[i];
        if (r > e / 2) break;
        if (is_prime[e - r]) {
            gold_ex[e] = 1;
            return true;
        }
    }
    gold_ex[e] = 0;
    return false;
}

/* ------------------------------------------------------------------ */
/* Next odd prime helper -------------------------------------------- */

static seq_t next_odd_prime(seq_t p)
{
    /* Candidate must be odd and > p */
    seq_t cand = p + (seq_t)2;
    for (; cand <= (seq_t)SIEVE_LIMIT; cand += (seq_t)2) {
        if (is_prime[(size_t)cand]) return cand;
    }

    /* If beyond sieve, simply test primality by trial division */
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t pr = (seq_t)primes[i];
            if (pr * pr > cand) break;
            if (cand % pr == 0) { ok = false; break; }
        }
        if (ok) return cand;
        cand += (seq_t)2;
    }
}

/* ------------------------------------------------------------------ */
/* Sequence computation ---------------------------------------------- */

static void compute_sequence(void)
{
    seq_cache = (seq_t*)malloc(primes_cnt * sizeof(seq_t));

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t best_q = (seq_t)0;

        for (size_t qidx = i; qidx > 0; --qidx) {
            seq_t q = (seq_t)primes[qidx - 1];
            seq_t e = p - q;      /* even number, guaranteed by parity */
            if (has_gold_breakdown((idx_t)e)) {
                best_q = q;
                break;           /* largest q found */
            }
        }

        if (best_q == (seq_t)0) {  /* no decomposition found */
            if (i + 1 < primes_cnt)
                best_q = (seq_t)primes[i + 1];
            else
                best_q = next_odd_prime(p);
        }

        seq_cache[i] = best_q;
    }
}

/* ------------------------------------------------------------------ */
/* Public API -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined, safety */

    if (!initialized) {
        init_sieve();
        compute_sequence();
        initialized = true;
    }

    if ((idx_t)(n - 1) >= primes_cnt) return (seq_t)0;   /* out of range */

    return seq_cache[n - 1];
}
