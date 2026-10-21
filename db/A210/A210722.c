/*
 * A210722  Number of ways to write n = (2-(n mod 2))p+q+2^k with p, q-1, q+1
 *          all prime, and p-1, p+1, q all practical.
 * https://oeis.org/A210722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to write n = (2-(n mod 2))p + q + 2^k with p
 *            prime, q-1 and q+1 prime, p-1, p+1 and q practical
 * Range:     u8 n <= 668, u16 n <= 9680, u32 n <= 9680, u64 n <= 9680,
 *            u128 n <= 9680, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

/* ----- static data for memoisation ------------------------------------ */
static idx_t precomp_limit = 0;
static bool *is_practical  = NULL;   /* 0 .. limit+1 */
static bool *prime_flag    = NULL;   /* 0 .. limit+1 */
static idx_t *spf           = NULL;   /* smallest prime factor for 0 .. limit+1 */
static idx_t *primes        = NULL;   /* list of primes up to limit      */
static size_t primes_count  = 0;
static idx_t *p_cands        = NULL;   /* primes with p-1 and p+1 practical */
static size_t p_cands_cnt    = 0;
static idx_t *q_cands        = NULL;   /* practical q with q-1 and q+1 prime */
static size_t q_cands_cnt    = 0;

/* ----- helper --------------------------------------------------------- */

/* ----- big integer sigma factor for prime power --------------------------------- */
static seq_t sigma_prime_power(idx_t p, int e)
{
    seq_t sigma = 1;
    seq_t power = 1;
    for (int i = 0; i < e; ++i) {
        power *= p;
        sigma += power;
    }
    return sigma;
}

/* ----- test practicability ------------------------------------------------ */
static bool is_practical_n(idx_t n)
{
    if (n == 0) return false;
    if (n == 1) return true;

    idx_t x = n;
    idx_t prev_prime = 0;
    seq_t sigma_prev = 1;
    bool ok = true;

    while (x > 1) {
        idx_t p = spf[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        if (prev_prime == 0) {
            /* first prime factor must be 2 */
            if (p != 2) { ok = false; break; }
        } else {
            if (p > sigma_prev + 1) { ok = false; break; }
        }

        seq_t sigma_fact = sigma_prime_power(p, exp);
        sigma_prev *= sigma_fact;
        prev_prime = p;
    }

    return ok;
}

/* ----- pre‑compute tables up to 'limit' ------------------------------------
   The function is idempotent: it will not recompute if an earlier call
   already covered 'limit'.                                                     */
static void precompute(idx_t limit)
{
    if (limit <= precomp_limit) return;  /* already sufficient */

    /* free old data */
    free(is_practical);
    free(prime_flag);
    free(spf);
    free(primes);
    free(p_cands);
    free(q_cands);

    idx_t ext = limit + 1;   /* we need to test up to n+1 for p+1 and q+1 */

    /* ---- smallest prime factors ------------------------------------- */
    spf = (idx_t *)malloc((size_t)(ext + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= ext; ++i) spf[i] = 0;
    spf[0] = 0; spf[1] = 1;
    for (idx_t i = 2; i <= ext; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > (ext / i)) continue;
            for (idx_t j = i * i; j <= ext; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= ext; ++i)
        if (spf[i] == 0) spf[i] = i;

    /* ---- prime flags ----------------------------------------------- */
    prime_flag = (bool *)malloc((size_t)(ext + 1) * sizeof(bool));
    prime_flag[0] = prime_flag[1] = false;
    for (idx_t i = 2; i <= ext; ++i)
        prime_flag[i] = (spf[i] == i);

    /* ---- practical numbers ------------------------------------------ */
    is_practical = (bool *)malloc((size_t)(ext + 1) * sizeof(bool));
    is_practical[0] = false;
    for (idx_t i = 1; i <= ext; ++i)
        is_practical[i] = is_practical_n(i);

    /* ---- primes list ----------------------------------------------- */
    primes_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) ++primes_count;
    primes = (idx_t *)malloc(primes_count * sizeof(idx_t));
    size_t pi = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (prime_flag[i]) primes[pi++] = i;
    }

    /* ---- candidate p: prime with p-1 and p+1 practical -------------- */
    p_cands_cnt = 0;
    p_cands = (idx_t *)malloc(primes_count * sizeof(idx_t));
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        if (p > limit) break;                 /* p cannot exceed n */
        if (p + 1 <= ext && p - 1 <= ext &&
            is_practical[p - 1] && is_practical[p + 1]) {
            p_cands[p_cands_cnt++] = p;
        }
    }

    /* ---- candidate q: practical q with q-1 and q+1 prime -------------- */
    q_cands_cnt = 0;
    q_cands = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    for (idx_t q = 1; q <= limit; ++q) {
        if (is_practical[q] &&
            q >= 2 && (q - 1) <= ext && (q + 1) <= ext &&
            prime_flag[q - 1] && prime_flag[q + 1]) {
            q_cands[q_cands_cnt++] = q;
        }
    }

    precomp_limit = limit;
}

/* ----- public sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used but safe */

    precompute(n);                  /* ensure tables cover n          */
    seq_t count = 0;

    /* m = 2 if n even, else 1 */
    idx_t m = (n & 1) ? 1ULL : 2ULL;

    for (size_t ip = 0; ip < p_cands_cnt; ++ip) {
        idx_t p = p_cands[ip];
        idx_t mp = m * p;
        if (mp > n) continue;
        idx_t rem_start = n - mp;
        if (rem_start < 5) continue; /* minimal q+2^k needed */

        for (size_t iq = 0; iq < q_cands_cnt; ++iq) {
            idx_t q = q_cands[iq];
            if (q > rem_start - 1) break;   /* q too large for this p */
            idx_t r = rem_start - q;
            if (is_power_of_two(r)) ++count;
        }
    }
    return count;
}
