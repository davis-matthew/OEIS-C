/*
 * A156119  Primes formed by rearranging five consecutive decimal digits
 *          (avoiding leading 0).
 * https://oeis.org/A156119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) is the nth prime that can be obtained by permuting the five
 *            consecutive decimal digits d, d+1, d+2, d+3, d+4 (for some d
 *            with 0 ≤ d ≤ 5) and that does not have a leading zero. The
 *            sequence is finite (52 terms) and sorted increasingly
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t primes[600];
static size_t primes_len = 0;

/* compare function for qsort */
static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* recursive permutation generator */
static void permute(int idx, uint8_t *dig) {
    if (idx == 5) {
        if (dig[0] == 0) return;          /* avoid leading zero       */
        seq_t val = 0;
        for (int i = 0; i < 5; ++i) {
            val = val * 10 + dig[i];
        }
        if (isPrimeGeneric(val)) {
            primes[primes_len++] = val;
        }
        return;
    }
    for (int i = idx; i < 5; ++i) {
        uint8_t tmp = dig[idx];
        dig[idx] = dig[i];
        dig[i] = tmp;
        permute(idx + 1, dig);
        /* backtrack */
        tmp = dig[idx];
        dig[idx] = dig[i];
        dig[i] = tmp;
    }
}

/* generate all primes of the sequence (called at first use) */
static void generate_sequence(void) {
    for (uint8_t d = 0; d <= 5; ++d) {
        uint8_t digits[5];
        for (int i = 0; i < 5; ++i)
            digits[i] = d + i;
        permute(0, digits);
    }
    if (primes_len > 0) {
        qsort(primes, primes_len, sizeof(seq_t), cmp_seq);
    }
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n = 0 (offset 1) */
    if (primes_len == 0) {
        generate_sequence();
    }
    if (n - 1 < primes_len)
        return primes[n - 1];
    return 0; /* should not happen for valid n */
}
