/*
 * A094753  Largest k such that (2n-1)/k + 2 as well as (2n-1)k + 2 are prime,
 *          or 0 if no such number exists.
 * https://oeis.org/A094753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 134, u32 n <= 152, u64 n <= 152,
 *            u128 n <= 152, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A094753: Largest k such that ((2n-1)/k)+2 and ((2n-1)*k)+2 are
 * prime, or 0 if no such k exists.  k must be a positive divisor of (2n-1).
 * The sequence is computed by iterating over all divisors of 2n-1 and
 * checking the two primality conditions.
 */

#include <stdint.h>
#include <stdbool.h>

/* Sieve upper bound: needs primes up to sqrt((2n-1)^2+2) for n≤10000
 * (≈ sqrt(4×10^8) ≈ 20000).  20000 is ample. */
#define SIEVE_MAX 20000

/* static data for the prime sieve */
static bool sieve_done = false;
static bool is_prime_small[SIEVE_MAX + 1];
static int  prime_list[3000];
static int  prime_count = 0;

/* initialise sieve once */
static void init_sieve(void)
{
    if (sieve_done) return;
    for (int i = 0; i <= SIEVE_MAX; ++i) is_prime_small[i] = true;
    is_prime_small[0] = is_prime_small[1] = false;
    for (int p = 2; p * p <= SIEVE_MAX; ++p) {
        if (is_prime_small[p]) {
            for (int m = p * p; m <= SIEVE_MAX; m += p)
                is_prime_small[m] = false;
        }
    }
    for (int i = 2; i <= SIEVE_MAX; ++i)
        if (is_prime_small[i]) prime_list[prime_count++] = i;
    sieve_done = true;
}

/* simple integer square root (floor) for unsigned 64‑bit numbers */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* primality test using the prime list.  Works for numbers up to
 * (2n-1)^2+2 (≈4×10^8). */
static inline bool is_prime_int(int x)
{
    if (x <= SIEVE_MAX) return is_prime_small[x];
    for (int i = 0; i < prime_count && prime_list[i] * prime_list[i] <= x; ++i)
        if (x % prime_list[i] == 0) return false;
    return true;
}

/* check whether a particular divisor k satisfies the two prime conditions */
static inline bool ok_k(idx_t k, idx_t m)
{
    seq_t p1 = (seq_t)m / k + 2;
    seq_t p2 = (seq_t)m * k + 2;
    /* p1 and p2 fit in 32 bits for this problem size */
    return is_prime_int((int)p1) && is_prime_int((int)p2);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_sieve();

    idx_t m = 2 * n - 1;            /* 2n-1 */
    seq_t best = 0;

    idx_t root = isqrt_idx(m);
    for (idx_t d = 1; d <= root; ++d) {
        if (m % d == 0) {
            idx_t k1 = d;
            if (ok_k(k1, m) && k1 > best) best = k1;

            idx_t k2 = m / d;
            if (k2 != k1) {
                if (ok_k(k2, m) && k2 > best) best = k2;
            }
        }
    }
    return best;
}
