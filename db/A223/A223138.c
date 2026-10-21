/*
 * A223138  Numbers n such that sigma(n+1) - sigma(n-1) = n / k for some
 *          integer, where sigma(n) = A000203 (sum of divisors of n).
 * https://oeis.org/A223138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223138: a(n) is the n‑th integer such that
   sigma(n+1) – sigma(n‑1) divides n, i.e.
   there exists an integer k with sigma(n+1)-sigma(n‑1)=n/k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- helpers ----- */

/* Compute floor(sqrt(x)) for a 64‑bit unsigned integer. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;          /* 2^32 > sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Prime generation up to a given limit. */
static uint32_t *primes = NULL;
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;
static idx_t   prime_limit = 1;   /* largest number tested for primality */

static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool is_p = true;
        idx_t sq = _isqrt_u64(cand);
        for (size_t i = 0; i < primes_cnt && primes[i] <= sq; ++i) {
            if (cand % primes[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)cand;
        }
    }
    if (limit > prime_limit) prime_limit = limit;
}

/* Sum of divisors function sigma(n).  sigma(0) is defined as 0. */
static seq_t _sigma_u64(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;
    idx_t temp   = n;
    idx_t need   = _isqrt_u64(temp) + 1;
    _ensure_primes_upto(need);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t pow  = 1;
            while (temp % p == 0) {
                temp /= p;
                pow  *= (seq_t)p;
                term += pow;
            }
            result *= term;
        }
    }
    if (temp > 1) result *= ((seq_t)temp + 1);
    return result;
}

/* ----- sequence storage ----- */
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;          /* number of terms stored */
static size_t seq_cap  = 0;          /* allocated capacity */
static idx_t  last_candidate = 1;    /* last n that was tested */

/* Append a new term to the array, growing it if needed. */
static void _append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
    seq_vals[seq_len++] = val;
}

/* ----- public function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    while (seq_len < n) {
        idx_t cand = last_candidate + 1;   /* next integer to test */

        /* need sigma for cand-1 and cand+1 */
        idx_t low  = cand - 1;
        idx_t high = cand + 1;
        idx_t max_needed = low > high ? low : high;
        idx_t sqrt_needed = _isqrt_u64(max_needed) + 1;
        _ensure_primes_upto(sqrt_needed);

        seq_t sigma_low  = _sigma_u64(low);
        seq_t sigma_high = _sigma_u64(high);
        seq_t diff = sigma_high > sigma_low ? sigma_high - sigma_low
                                            : sigma_low - sigma_high;

        /* diff must be positive, divide cand */
        if (diff > 0 && diff <= (seq_t)cand) {
            idx_t diff_i = (idx_t)diff;   /* safe: diff <= cand <= UINT64_MAX */
            if (cand % diff_i == 0) {
                _append_term(cand);
            }
        }
        last_candidate = cand;
    }
    return seq_vals[n - 1];
}
