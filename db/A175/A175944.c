/*
 * A175944  1 appears once, n-th prime p appears p times.
 * https://oeis.org/A175944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175944:
 * a(1) = 1, for n > 1 each prime p appears p times in the sequence.
 * Thus a(n) is the prime p for which the cumulative sum of all previous
 * primes is < n-1 <= the cumulative sum including p. */

#include <stdint.h>
#include <stdlib.h>

/* Simple primality test suitable for the small range of primes needed. */
static int is_prime_uint64(uint64_t x) {
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return 0;
    }
    return 1;
}

/* Return the n-th term of A175944. */
seq_t a(idx_t n) {
    static uint64_t *prime_arr = NULL;   /* list of primes */
    static seq_t *cum_arr   = NULL;      /* cumulative sums of primes */
    static size_t prime_cnt = 0;
    static size_t cum_cnt   = 0;
    static uint64_t last_candidate = 1;   /* start searching for primes from 2 */

    if (n == 1) return (seq_t)1;          /* the special first term */

    idx_t m = n - 1;                      /* number of terms after the first */

    /* Extend the prime list until the cumulative sum covers m. */
    while (cum_cnt == 0 || (seq_t)m > cum_arr[cum_cnt - 1]) {
        /* Find next prime. */
        do {
            ++last_candidate;
        } while (!is_prime_uint64(last_candidate));

        uint64_t p = last_candidate;

        /* Append to prime array. */
        uint64_t *tmp_prime = (uint64_t *)realloc(prime_arr,
                                      sizeof(uint64_t) * (prime_cnt + 1));
        if (!tmp_prime) {                 /* allocation failure – abort */
            abort();
        }
        prime_arr = tmp_prime;
        prime_arr[prime_cnt] = p;

        /* Append to cumulative sum array. */
        seq_t *tmp_cum = (seq_t *)realloc(cum_arr,
                          sizeof(seq_t) * (cum_cnt + 1));
        if (!tmp_cum) {
            abort();
        }
        cum_arr = tmp_cum;
        if (cum_cnt == 0) {
            cum_arr[0] = (seq_t)p;
        } else {
            cum_arr[cum_cnt] = cum_arr[cum_cnt - 1] + (seq_t)p;
        }

        ++prime_cnt;
        ++cum_cnt;
    }

    /* Binary search to locate the prime that covers position m. */
    size_t left = 0, right = cum_cnt - 1;
    while (left < right) {
        size_t mid = (left + right) / 2;
        if ((seq_t)m <= cum_arr[mid]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return (seq_t)prime_arr[left];
}
