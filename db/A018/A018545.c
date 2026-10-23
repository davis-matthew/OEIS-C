/*
 * A018545  Divisors of 605.
 * https://oeis.org/A018545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 605 sorted ascending
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void gen_divs(const uint64_t *primes, const uint32_t *exps, size_t mp, size_t idx,
                     seq_t cur, seq_t *out, size_t *pos) {
    if (idx == mp) {
        out[*pos] = cur;
        ++(*pos);
        return;
    }
    seq_t val = 1;
    for (uint32_t e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, mp, idx + 1, cur * val, out, pos);
        val = val * (seq_t)primes[idx];
    }
}

static void sort_seq_t(seq_t *arr, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

static bool initialized = false;
static seq_t divs[20];    /* enough for 6 divisors */
static size_t divcnt = 0;

seq_t a(idx_t n) {
    if (!initialized) {
        uint64_t primes[10];
        uint32_t exps[10];
        size_t mp = 0;
        uint64_t temp = 605;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                primes[mp] = p;
                uint32_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                exps[mp] = e;
                ++mp;
            }
        }
        if (temp > 1) {
            primes[mp] = temp;
            exps[mp] = 1;
            ++mp;
        }
        divcnt = 1;
        for (size_t i = 0; i < mp; ++i) divcnt *= (exps[i] + 1);
        size_t pos = 0;
        gen_divs(primes, exps, mp, 0, (seq_t)1, divs, &pos);
        sort_seq_t(divs, divcnt);
        initialized = true;
    }
    if (n >= 1 && (idx_t)n <= divcnt) {
        return divs[(size_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
