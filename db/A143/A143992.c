/*
 * A143992  Numbers k such that the sum of the prime factors of k (counted
 *          with multiplicity) is a proper substring of k.
 * https://oeis.org/A143992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A143992: numbers k such that the sum of prime factors of k
   (counted with multiplicity) is a proper substring of k. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline void uint64_to_str(uint64_t n, char *buf, int *len) {
    int i = 0;
    if (n == 0) {
        buf[i++] = '0';
    } else {
        while (n > 0) {
            buf[i++] = '0' + (n % 10);
            n /= 10;
        }
        /* reverse to get correct order */
        for (int j = 0; j < i / 2; ++j) {
            char t = buf[j];
            buf[j] = buf[i - 1 - j];
            buf[i - 1 - j] = t;
        }
    }
    *len = i;
}

/* Sum of prime factors of n, counted with multiplicity. */
static inline uint64_t sum_prime_factors_uint64(uint64_t n) {
    uint64_t sum = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        while (m % p == 0) {
            sum += p;
            m /= p;
        }
    }
    if (m > 1) sum += m;   /* last prime factor */
    return sum;
}

/* Returns true if sum is a proper substring of k. */
static inline bool is_sum_proper_substring(uint64_t k, uint64_t sum) {
    if (sum == 0 || sum == k) return false;   /* proper substring */

    char kbuf[21], sbuf[21];
    int klen, slen;
    uint64_to_str(k, kbuf, &klen);
    uint64_to_str(sum, sbuf, &slen);

    if (slen > klen) return false;

    for (int i = 0; i <= klen - slen; ++i) {
        bool ok = true;
        for (int j = 0; j < slen; ++j) {
            if (kbuf[i + j] != sbuf[j]) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

/* External function required by the build system. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_k = 1;

    if (n == 0) {
        return 0;   /* undefined for n=0, but return safe value */
    }

    while (cache_len < n) {
        uint64_t k = next_k;
        uint64_t sum = sum_prime_factors_uint64(k);
        if (is_sum_proper_substring(k, sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
