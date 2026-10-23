/*
 * A046116  Numbers k such that no Paley-type Hadamard matrix of order k
 *          exists.
 * https://oeis.org/A046116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisible by 4 but not of form 2^a*(q+1), q = odd prime power. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- integer square root (uint64_t) ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, ans = 0;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        if (mid == 0) {                      /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= n / mid) {                /* mid*mid <= n   */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* ---------- simple sieve for generating primes up to limit ---------- */
static uint64_t *prime_list      = NULL;
static size_t    prime_count     = 0;
static uint64_t  prime_limit     = 0;

static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;

    size_t size = limit + 1;
    unsigned char *sieve = (unsigned char*)malloc(size);
    if (!sieve) exit(1);
    memset(sieve, 1, size);
    if (size) sieve[0] = 0;
    if (size > 1) sieve[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                sieve[j] = 0;
            }
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (sieve[i]) new_list[idx++] = i;

    free(sieve);
    if (prime_list) free(prime_list);
    prime_list  = new_list;
    prime_count = cnt;
    prime_limit = limit;
}

/* ---------- check whether n (odd, >=3) is an odd prime power ---------- */
static bool odd_prime_power(seq_t n)
{
    if (n < 3) return false;
    if (((seq_t)n & 1) == 0) return false;          /* even */

    uint64_t x   = (uint64_t)n;
    uint64_t lim = isqrt_u64(x);
    if (lim > prime_limit) generate_primes_up_to(lim);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > lim) break;
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            return x == 1;                         /* prime power if remainder 1 */
        }
    }
    /* no divisor found – x is prime (odd) */
    return true;
}

/* ---------- test whether a multiple of 4 is excluded ---------- */
static bool is_excluded(uint64_t k)
{
    if ((k & 3) != 0) return false;                /* not divisible by 4 */

    uint64_t temp = k;
    uint64_t exp  = 0;
    while ((temp & 1) == 0) {                     /* count trailing zeros */
        temp >>= 1;
        ++exp;
    }

    uint64_t m = k;
    for (uint64_t a = 0; a <= exp; ++a) {
        if (odd_prime_power((seq_t)(m - 1))) return true;  /* excluded */
        m >>= 1;
    }
    return false;                                 /* allowed */
}

/* ---------- cache for computed sequence values ---------- */
struct seq_state_t {
    seq_t  *cache;      /* stored sequence */
    size_t len;         /* number of stored terms */
    size_t cap;         /* capacity of cache */
    uint64_t last_checked; /* last multiple of 4 examined */
} seq_state = {NULL, 0, 0, 0};

static void grow_cache(size_t needed)
{
    if (needed <= seq_state.cap) return;
    size_t new_cap = seq_state.cap == 0 ? 16 : seq_state.cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (seq_state.cache) {
        memcpy(new_cache, seq_state.cache, seq_state.len * sizeof(seq_t));
        free(seq_state.cache);
    }
    seq_state.cache = new_cache;
    seq_state.cap   = new_cap;
}

/* ---------- main function to return the nth term ---------- */
seq_t a(idx_t n)
{
    /* generate terms until we have n of them */
    while (seq_state.len < (size_t)n) {
        seq_state.last_checked += 4;              /* next multiple of 4 */
        if (!is_excluded(seq_state.last_checked)) {
            grow_cache(seq_state.len + 1);
            seq_state.cache[seq_state.len++] = (seq_t)seq_state.last_checked;
        }
    }
    return seq_state.cache[(size_t)n - 1];
}
