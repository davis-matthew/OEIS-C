/*
 * A188173  Squarefree numbers m such that the equation x^2 == m (mod 10^k)
 *          has solutions for all k > 0.
 * https://oeis.org/A188173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188173: squarefree numbers m such that x^2 == m (mod 10^k)
 * for all k > 0.  The set consists exactly of the square‑free integers
 * congruent to 1 or 9 modulo 40.  The function caches generated terms
 * so that calls with arbitrary order of indices remain O(1) after the
 * initial generation. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* --- small integer square root for uint64_t --------------------------------*/
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t s = (uint64_t)sqrtl((long double)n);
    while ((s + 1) * (s + 1) <= n) ++s;
    while (s * s > n) --s;
    return s;
}

/* --- dynamic prime list used for trial division ---------------------------*/
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;

/* Ensure that prime_list contains all primes up to `limit`. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) {              /* initialise with the prime 2 */
        prime_cnt = 1;
        prime_cap  = 8;
        prime_list = (uint64_t*)realloc(NULL, prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
    }
    uint64_t cand = prime_list[prime_cnt - 1] + (prime_list[prime_cnt - 1] == 2 ? 1 : 2);
    while (cand <= limit) {
        bool is_p = true;
        uint64_t sq = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 8;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
        cand += 2;
    }
}

/* Check whether a uint64_t number is square‑free. */
static bool is_squarefree_u64(uint64_t n)
{
    if (n <= 1) return true;
    uint64_t limit = isqrt_u64(n);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            if ((n / p) % p == 0) return false;
        }
    }
    return true;
}

/* --- cache for generated sequence terms ---------------------------------*/
static seq_t *seq_cache = NULL;
static size_t seq_cached = 0, seq_cap = 0;
static uint64_t next_candidate = 1;   /* next integer to test */

/* Ensure the sequence cache can hold at least `need` items. */
static void ensure_seq_cache(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below offset is irrelevant */

    size_t target = (size_t)n;
    while (seq_cached < target) {
        ++next_candidate;
        if (next_candidate <= 1) continue;               /* m = 1 is omitted */

        uint64_t r = next_candidate % 40;
        if (r != 1 && r != 9) continue;                  /* wrong congruence */

        if (!is_squarefree_u64(next_candidate))
            continue;                                   /* not squarefree */

        ensure_seq_cache(seq_cached + 1);
        seq_cache[seq_cached++] = (seq_t)next_candidate;
    }
    return seq_cache[n - 1];
}
