/*
 * A163405  a(n) = (n-th nonprime) + (number of bits in binary expansion of
 *          n-th nonprime).
 * https://oeis.org/A163405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th nonprime) + (number of bits in binary expansion of
 *            n-th nonprime)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* static cache for sieve and prime prefix counts */
static idx_t current_sieve_limit = 0;
static bool *prime_flags = NULL;
static idx_t *prime_prefix = NULL;

/* build or extend sieve up to 'limit' */
static void update_sieve(idx_t limit) {
    if (limit <= current_sieve_limit) return;
    free(prime_flags);
    free(prime_prefix);
    prime_flags = (bool*)malloc((limit + 1) * sizeof(bool));
    prime_prefix = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) prime_flags[i] = true;
    if (limit >= 0) prime_flags[0] = false;
    if (limit >= 1) prime_flags[1] = false;
    idx_t sq = (idx_t)(sqrt((double)limit) + 1.0);
    for (idx_t i = 2; i <= sq; ++i) {
        if (prime_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i) prime_flags[j] = false;
        }
    }
    idx_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (prime_flags[i]) ++cnt;
        prime_prefix[i] = cnt;
    }
    current_sieve_limit = limit;
}

/* helper: return nth nonprime (1‑based) using binary search on the sieve */
static idx_t nth_nonprime(idx_t n) {
    idx_t low = 0, high = current_sieve_limit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t nonprime_cnt = (mid + 1) - prime_prefix[mid];
        if (nonprime_cnt >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* helper: number of bits in binary representation of a non‑negative 64‑bit integer */
static int bitlen_uint64(uint64_t x) {
    if (x == 0) return 1;
    int len = 0;
    while (x) { ++len; x >>= 1; }
    return len;
}

/* required function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* offset safety */
    idx_t limit = 2 * n + 100;                /* enough to contain nth nonprime */
    if (limit < 100) limit = 100;             /* minimal size for small n */
    update_sieve(limit);
    idx_t np = nth_nonprime(n);               /* the nth nonprime number */
    int bits = bitlen_uint64((uint64_t)np);   /* bits in binary expansion */
    return (seq_t)np + (seq_t)bits;
}
