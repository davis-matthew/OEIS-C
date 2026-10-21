/*
 * A096512  Numbers k such that A096509(k) = 1; i.e., the number of prime
 *          powers (including primes) in the neighborhood of k with radius
 *          ceiling(log(k)) is 1.
 * https://oeis.org/A096512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th integer k such that the number of
 * prime powers (including primes) in the interval [k - r, k + r]
 * with r = ceil(log(k)) is exactly 1.  The function scans k from 1
 * upward, maintains a sieve of prime powers up to the largest needed
 * bound, and keeps a prefix sum array to answer counts in O(1).  This
 * algorithm runs in essentially linear time with respect to the
 * largest k examined. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Internal tables used for computing ceil(log(k)).  The table holds
 * e^i for i = 0 .. 99; this is sufficient because k will never
 * exceed 1e18 in the test harness. */
static long double exp_table[100];
static bool exp_table_inited = false;
static void init_exp_table(void)
{
    if (exp_table_inited) return;
    for (size_t i = 0; i < 100; ++i) exp_table[i] = expl((long double)i);
    exp_table_inited = true;
}
static inline uint64_t ln_ceil(uint64_t k)
{
    if (k <= 1) return 0;
    init_exp_table();
    size_t i = 0;
    while (exp_table[i] < (long double)k) ++i;
    return (uint64_t)i;
}

/* Sieve state ----------------------------------------------------- */
static size_t sieve_limit   = 0;     /* largest index for which sieve is valid */
static bool*   is_prime_power = NULL;/* is i a prime power? */
static size_t* prefix        = NULL;/* cumulative counts of prime powers */

/* Recompute the sieve up to 'limit' (inclusive). */
static void recompute_sieve(size_t limit)
{
    /* 1. Simple sieve of primes up to limit */
    bool* is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* 2. Mark prime powers */
    free(is_prime_power);
    is_prime_power = (bool*)calloc(limit + 1, sizeof(bool));
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        uint64_t val = p;
        while (val <= limit) {
            is_prime_power[(size_t)val] = true;
            if (val > limit / p) break;      /* avoid overflow */
            val *= p;
        }
    }
    free(is_prime);

    /* 3. Build prefix sum array */
    free(prefix);
    prefix = (size_t*)malloc((limit + 1) * sizeof(size_t));
    prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        prefix[i] = prefix[i - 1] + (is_prime_power[i] ? 1 : 0);
    }

    sieve_limit = limit;
}

/* Ensure prime power data up to at least 'need'.  If the current
 * array is not large enough, grow it (doubling strategy). */
static inline void ensure_sieve(size_t need)
{
    if (need <= sieve_limit) return;
    size_t new_limit = need < 64 ? 64 : need;
    while (new_limit < need) new_limit <<= 1;
    recompute_sieve(new_limit);
}

/* Sequence storage -------------------------------------------------- */
static size_t seq_len   = 0;     /* number of values found so far */
static size_t seq_cap   = 0;     /* allocated capacity */
static seq_t* seq_vals  = NULL; /* array of found values */

/* Append a value to the sequence storage, resizing if necessary. */
static void append_seq_value(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t* new_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_vals = new_vals;
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    static uint64_t current_k = 1;   /* next k to examine */

    while (seq_len < n) {
        uint64_t k = current_k;
        uint64_t r = ln_ceil(k);

        uint64_t L = (k > r) ? k - r : 1;          /* lower bound of neighborhood */
        uint64_t R = k + r;                       /* upper bound */

        ensure_sieve((size_t)R);                   /* make sure sieve covers R */

        size_t count = prefix[(size_t)R] - ((L > 1) ? prefix[(size_t)L - 1] : 0);
        if (count == 1) {
            append_seq_value((seq_t)k);
        }
        ++current_k;
    }

    return seq_vals[n - 1];
}
