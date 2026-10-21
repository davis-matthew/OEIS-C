/*
 * A077389  Smallest integer that is the average of n consecutive primes.
 * https://oeis.org/A077389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer that is the average of n consecutive
 *            primes. Computation: For each n, generate primes sequentially
 *            and maintain a sliding window of the last n primes. The first
 *            window whose sum is divisible by n gives the desired average
 * Range:     u8 n <= 4, u16 n <= 226, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Prime storage */
static uint32_t *prime_buf = NULL;
static size_t prime_cap = 0, prime_cnt = 0;
static uint32_t prime_candidate = 2;

/* Ensure that prime_buf contains primes[0 .. idx] */
static void ensure_prime_at(size_t idx)
{
    while (prime_cnt <= idx) {
        /* Generate next prime */
        while (1) {
            int is_p = 1;
            uint32_t limit = (uint32_t)sqrt((double)prime_candidate);
            for (size_t i = 0; i < prime_cnt && prime_buf[i] <= limit; ++i) {
                if (prime_candidate % prime_buf[i] == 0) {
                    is_p = 0;
                    break;
                }
            }
            if (is_p) {
                break;          /* prime_candidate is prime */
            }
            ++prime_candidate;
        }
        if (prime_cnt == prime_cap) {
            prime_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
            prime_buf = (uint32_t*)realloc(prime_buf, prime_cap * sizeof(uint32_t));
        }
        prime_buf[prime_cnt++] = prime_candidate;
        ++prime_candidate;
    }
}

/* Global cache of computed a(n) values */
static seq_t *ans_buf = NULL;
static size_t ans_cap = 0, ans_cnt = 0;

static void ans_ensure(size_t n)
{
    if (n < ans_cnt) return;
    if (n >= ans_cap) {
        ans_cap = (ans_cap == 0) ? 64 : ans_cap * 2;
        ans_buf = (seq_t*)realloc(ans_buf, ans_cap * sizeof(seq_t));
    }
    for (; ans_cnt <= n; ++ans_cnt)
        ans_buf[ans_cnt] = (seq_t)-1;   /* mark as uncomputed */
}

/* Core function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    size_t s_n = (size_t)n;        /* n fits in size_t for our ranges */
    ans_ensure(s_n);
    if (ans_buf[s_n] != (seq_t)-1) return ans_buf[s_n];

    /* Build initial window of size s_n */
    ensure_prime_at(s_n - 1);
    seq_t sum = 0;
    for (size_t j = 0; j < s_n; ++j) sum += (seq_t)prime_buf[j];
    if (sum % (seq_t)n == 0) {
        ans_buf[s_n] = sum / (seq_t)n;
        return ans_buf[s_n];
    }

    size_t i = 1;
    while (1) {
        /* Ensure we have prime at position i + s_n - 1 */
        ensure_prime_at(i + s_n - 1);
        sum += (seq_t)prime_buf[i + s_n - 1];
        sum -= (seq_t)prime_buf[i - 1];
        if (sum % (seq_t)n == 0) {
            ans_buf[s_n] = sum / (seq_t)n;
            return ans_buf[s_n];
        }
        ++i;
    }
}
