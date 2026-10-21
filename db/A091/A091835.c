/*
 * A091835  Double factorial of primes.
 * https://oeis.org/A091835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)!!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square‑root for uint64_t, used for trial division. */
static inline idx_t int_isqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) <= x / (r + 1))
        ++r;
    return r;
}

/* Prime cache and corresponding double‑factorial values. */
static uint64_t *primes     = NULL;
static size_t   primes_cnt  = 0;
static seq_t   *dvalues     = NULL;
static uint64_t next_candidate = 2;

/* Ensure that we have at least n primes and their double‑factorials cached. */
static void ensure_n(idx_t n)
{
    if (n < 1 || primes_cnt >= (size_t)n)
        return;

    /* Grow capacity if necessary. */
    size_t need = (size_t)n;
    size_t cap  = primes_cnt ? primes_cnt : 16;
    while (cap < need)
        cap <<= 1;

    uint64_t *tmp_p  = (uint64_t*)realloc(primes,  cap * sizeof(uint64_t));
    if (!tmp_p) abort();
    primes = tmp_p;

    seq_t   *tmp_v  = (seq_t*)realloc(dvalues, cap * sizeof(seq_t));
    if (!tmp_v) abort();
    dvalues = tmp_v;

    /* Generate missing primes and their double‑factorials. */
    for (size_t i = primes_cnt; i < need; ++i) {
        uint64_t cand = next_candidate;
        while (true) {
            bool is_p = true;
            idx_t limit = int_isqrt(cand);
            for (size_t j = 0; j < primes_cnt && (idx_t)primes[j] <= limit; ++j) {
                if (cand % primes[j] == 0) {
                    is_p = false;
                    break;
                }
            }
            if (is_p)
                break;
            ++cand;
        }

        primes[primes_cnt] = cand;
        ++primes_cnt;
        next_candidate = cand + 1;

        seq_t val;
        if (cand == 2) {
            val = 2;
        } else {
            val = 1;
            seq_t step = cand;
            while (true) {
                val *= step;
                if (step <= 2) break;
                step -= 2;
            }
        }
        dvalues[primes_cnt - 1] = val;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    ensure_n(n);
    return dvalues[n - 1];
}
