/*
 * A072864  Numbers m such that the smallest k dividing sigma(k*m^2) is equal
 *          to 3.
 * https://oeis.org/A072864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072864: numbers m such that the smallest k dividing σ(k·m²) is 3.  
   This is equivalent to “m has a prime factor p ≡ 1 (mod 6) whose
   exponent in m is exactly 1 (p² is not allowed)”.  The function
   searches for the n‑th such integer, memoizing all found values so
   that calls come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- dynamic array helpers ---------- */

static void ensure_capacity(size_t *cap, void **ptr, size_t elem_size, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* ---------- prime sieve helpers ---------- */

static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;

static void add_prime(uint64_t p)
{
    ensure_capacity(&prime_cap, (void **)&prime_list, sizeof(uint64_t), prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

static inline idx_t isqrt_u64(idx_t n)
{
    /* integer floor of sqrt(n) – binary search */
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid && mid > n / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) && (mid + 1) > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return n;
}

static void extend_primes_to(idx_t limit)
{
    if (prime_cnt > 0 && prime_list[prime_cnt - 1] >= limit)
        return;

    if (prime_cnt == 0) {
        add_prime(2);
        if (limit < 2)
            return;
    }

    for (idx_t candidate = prime_list[prime_cnt - 1] + 1; candidate <= limit; ++candidate) {
        if (candidate < 2)
            continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if ((idx_t)p * (idx_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(candidate);
    }
}

/* ---------- test whether a number has the required prime factor ---------- */

static bool has_required_prime_factor(idx_t m)
{
    idx_t tmp = m;
    extend_primes_to(isqrt_u64(tmp));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > tmp)
            break;
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (p % 6ULL == 1ULL && exp % 3ULL == 1ULL)
                return true;
        }
    }
    /* Remaining prime factor, if any */
    if (tmp > 1 && (tmp % 6ULL) == 1ULL)
        return true;
    return false;
}

/* ---------- cache of sequence values ---------- */

static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_checked = 0;

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1 */

    while (cache_len < n) {
        ++last_checked;
        if (has_required_prime_factor(last_checked)) {
            ensure_capacity(&cache_cap, (void **)&cache, sizeof(seq_t), cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
