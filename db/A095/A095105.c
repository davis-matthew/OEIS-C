/*
 * A095105  Length of maximal Dyck path prefix in the Legendre-vector of the
 *          n-th 4k+3 prime (A002145(n)).
 * https://oeis.org/A095105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of maximal Dyck path prefix in the Legendre vector
 *            of the n-th prime congruent to 3 mod 4 (A002145(n))
 * Range:     u8 n <= 29, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static helper variables for prime generation */
static idx_t    cur_candidate = 3;   /* next odd candidate to test */
static idx_t    prime3_count  = 0;   /* how many 4k+3 primes have been found */
static idx_t    prime3_value  = 0;   /* value of current nth 4k+3 prime */

/* small primes up to 1000 for trial division */
static uint32_t small_primes[200];
static size_t   small_cnt = 0;

/* initialize the list of small primes */
static void init_small_primes(void) {
    if (small_cnt > 0) return;
    bool is_composite[1001];
    memset(is_composite, 0, sizeof(is_composite));
    for (uint32_t i = 2; i <= 1000; ++i) {
        if (!is_composite[i]) {
            small_primes[small_cnt++] = i;
            if (i * i <= 1000) {
                for (uint32_t j = i * i; j <= 1000; j += i)
                    is_composite[j] = true;
            }
        }
    }
}

/* simple primality test using precomputed small primes */
static bool is_prime_uint64(idx_t x) {
    if (x < 2) return false;
    for (size_t k = 0; k < small_cnt && (uint64_t)small_primes[k] * small_primes[k] <= x; ++k) {
        if (x % small_primes[k] == 0) return x == small_primes[k];
    }
    return true;
}

/* get the n-th prime that is 3 mod 4 */
static idx_t nth_prime3(idx_t n) {
    init_small_primes();
    while (prime3_count < n) {
        while (!is_prime_uint64(cur_candidate) || (cur_candidate & 3) != 3)
            cur_candidate += 2;
        prime3_count++;
        prime3_value = cur_candidate;
        cur_candidate += 2;
    }
    return prime3_value;
}

seq_t a(idx_t n) {
    /* get the required prime */
    idx_t p = nth_prime3(n);
    /* array to mark quadratic residues modulo p */
    uint8_t *is_res = (uint8_t*)malloc((size_t)p * sizeof(uint8_t));
    if (!is_res) return 0;  /* allocation failure, return 0 */
    memset(is_res, 0, (size_t)p);
    /* mark all squares of 1 .. (p-1)/2 */
    idx_t half = (p - 1) >> 1;
    for (idx_t a = 1; a <= half; ++a) {
        idx_t sq = (a * a) % p;
        is_res[sq] = 1;
    }
    /* scan for maximal Dyck prefix */
    int64_t sum = 0;     /* cumulative walk, fits in 64-bit */
    idx_t   last_zero = 0;  /* length of last prefix that ends at 0 */
    for (idx_t i = 1; i < p; ++i) {
        sum += (is_res[i] ? 1 : -1);
        if (sum < 0) break;
        if (sum == 0) last_zero = i;
    }
    free(is_res);
    return (seq_t)last_zero;
}
