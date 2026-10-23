/*
 * A140612  Integers k such that both k and k+1 are the sum of 2 squares.
 * https://oeis.org/A140612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A140612:
 * Integers k such that both k and k+1 are sums of two squares.
 * The function generates the sequence by testing successive integers
 * and memoises the results so calls in any order are fast. */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* uint32_t */
#include <stdbool.h>  /* bool */

static inline idx_t isqrt_uint64(idx_t v)
{
    idx_t low = 0;
    idx_t high = v;
    seq_t v_seq = (seq_t)v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t prod = (seq_t)mid * (seq_t)mid;
        if (prod <= v_seq) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* ---------- Prime sieve (dynamic, incremental) ---------- */
static uint32_t *prime_list = NULL;
static size_t   prime_list_len = 0;
static size_t   prime_list_cap = 0;
static uint32_t last_checked_prime = 2;

static void grow_prime_list(void)
{
    size_t new_cap = prime_list_cap ? prime_list_cap << 1 : 16;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_list_cap = new_cap;
}

static void ensure_primes_upto(idx_t n)
{
    if ((idx_t)last_checked_prime > n) return;
    while ((idx_t)last_checked_prime <= n) {
        bool isp = true;
        idx_t limit = isqrt_uint64((idx_t)last_checked_prime);
        for (size_t i = 0; i < prime_list_len && prime_list[i] <= limit; ++i) {
            if (last_checked_prime % prime_list[i] == 0) {
                isp = false;
                break;
            }
        }
        if (isp) {
            if (prime_list_len == prime_list_cap) grow_prime_list();
            prime_list[prime_list_len++] = last_checked_prime;
        }
        ++last_checked_prime;
    }
}

/* ---------- Test for representation as a sum of two squares ---------- */
static bool is_sum_two_squares(seq_t x)
{
    if (x == (seq_t)0) return true;
    seq_t tmp = x;
    idx_t limit = isqrt_uint64((idx_t)tmp);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_list_len; ++i) {
        uint32_t p = prime_list[i];
        seq_t pp = (seq_t)p;
        if (pp > tmp / pp) break;               /* p^2 > tmp */
        if (tmp % pp == 0) {
            int exp = 0;
            while (tmp % pp == 0) {
                tmp /= pp;
                ++exp;
            }
            if ((p % 4) == 3 && (exp & 1)) return false;
        }
    }
    if (tmp > (seq_t)1) {                       /* remaining prime factor */
        if ((tmp % (seq_t)4) == (seq_t)3) return false;
    }
    return true;
}

/* ---------- Memoised sequence storage ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static idx_t next_k = 0;   /* next integer k to evaluate */

static void grow_seq_cache(void)
{
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* ---------- Public interface required by the framework ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* safe default for misuse */
    while (seq_cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t kp1 = k + (seq_t)1;
        if (is_sum_two_squares(k) && is_sum_two_squares(kp1)) {
            if (seq_cache_len == seq_cache_cap) grow_seq_cache();
            seq_cache[seq_cache_len++] = k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];                /* a(1) corresponds to seq_cache[0] */
}
