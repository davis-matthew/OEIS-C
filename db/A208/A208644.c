/*
 * A208644  Consider any chain of consecutive primes which divides n; take all
 *          longest such chains; maximize the product of the primes in the
 *          chain.
 * https://oeis.org/A208644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the product of the longest chain of consecutive prime *
 *            divisors of n; if several chains tie for maximum length, the
 *            maximum * product among them is returned. a(1)=1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

static uint64_t *spf = NULL;         /* smallest prime factor array */
static size_t spf_size = 0;          /* spf array size = spf_size elements */
static uint64_t *prime_list = NULL;  /* array of all primes up to current limit */
static size_t prime_cnt = 0;         /* number of primes in prime_list */

/* Find the index of prime p in prime_list by binary search.
   The prime p is guaranteed to be present because p <= current limit. */
static size_t find_prime_index(uint64_t p) {
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) {
            lo = mid + 1;
        } else if (prime_list[mid] > p) {
            hi = mid;
        } else {
            return mid;
        }
    }
    /* Should never happen */
    return (size_t)-1;
}

seq_t a(idx_t n) {
    if (n < 2) {
        return 1;
    }

    /* Ensure spf and prime list cover up to n */
    if (spf == NULL || spf_size <= (size_t)n) {
        if (spf != NULL) free(spf);
        build_spf((idx_t)n, &spf);
        spf_size = (size_t)n + 1;
    }
    if (prime_list == NULL || prime_cnt <= (size_t)n) {
        if (prime_list != NULL) free(prime_list);
        build_primes_upto((idx_t)n, &prime_list, &prime_cnt);
    }

    /* Extract distinct prime factors of n */
    uint64_t primes_buf[64];
    size_t pf_cnt = 0;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = spf[m];
        if (pf_cnt == 0 || primes_buf[pf_cnt - 1] != p) {
            primes_buf[pf_cnt++] = p;
        }
        while (m % p == 0) {
            m /= p;
        }
    }

    /* Find longest consecutive prime chain */
    size_t best_len = 0;
    seq_t best_prod = 1;

    size_t i = 0;
    while (i < pf_cnt) {
        size_t j = i;
        seq_t prod = (seq_t)primes_buf[j];
        size_t curr_idx = find_prime_index(primes_buf[j]);

        j++;
        while (j < pf_cnt) {
            size_t next_idx = find_prime_index(primes_buf[j]);
            if (next_idx == curr_idx + 1) {
                prod = prod * (seq_t)primes_buf[j];
                curr_idx = next_idx;
                j++;
            } else {
                break;
            }
        }

        size_t len = j - i;
        if (len > best_len || (len == best_len && prod > best_prod)) {
            best_len = len;
            best_prod = prod;
        }
        i = j;
    }

    return best_prod;
}
