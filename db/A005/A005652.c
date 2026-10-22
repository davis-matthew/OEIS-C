/*
 * A005652  Lexicographically least increasing sequence, starting with 1, such
 *          that the sum of 2 distinct terms is never a Fibonacci number.
 * https://oeis.org/A005652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th positive integer k with {k*phi} > 1/2, i.e. with k +
 *            floor(sqrt(5*k^2)) odd (since floor(2*k*phi) = k +
 *            floor(k*sqrt(5))); computed by a search
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005652_in_seq, a005652_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a005652_in_seq.h"
#include "../utils/a005652_isqrt.h"

static uint64_t *a005652_cache = NULL;
static idx_t a005652_len = 0;
static idx_t a005652_cap = 0;

seq_t a(idx_t n) {
    if (n > a005652_len) {
        if (n > a005652_cap) {
            idx_t newcap = a005652_cap ? a005652_cap : 64;
            while (newcap < n) newcap *= 2;
            a005652_cache = (uint64_t *)realloc(a005652_cache, sizeof(uint64_t) * newcap);
            a005652_cap = newcap;
        }
        uint64_t k = a005652_len ? a005652_cache[a005652_len - 1] : 0;
        while (a005652_len < n) {
            k++;
            if (a005652_in_seq(k)) {
                a005652_cache[a005652_len] = k;
                a005652_len++;
            }
        }
    }
    return (seq_t)a005652_cache[n - 1];
}
