/*
 * A104800  Primes containing sublist {9876543210}.
 * https://oeis.org/A104800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation of primes that contain the decimal substring 9876543210
   by exhaustive construction of all candidate numbers up to 16 digits
   and testing primality. The resulting list is sorted and indexed
   starting from 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "../utils/is_prime.h"

static seq_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_size  = 0;
static bool   precomputed = false;

/* Append a new prime value (uint64_t) to the dynamic array. */
static void push_prime(uint64_t val)
{
    if (prime_size == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_size++] = (seq_t)val;
}

/* Comparison function for qsort on seq_t. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Compute all primes containing 9876543210 up to 16 digits. */
static void precompute_primes(void)
{
    if (precomputed) return;

    static const uint8_t SUB[10] = {9,8,7,6,5,4,3,2,1,0};
    /* Precompute power of 10 up to 10^6 for loop limits */
    uint64_t pow10[7] = {1ULL};
    for (int i = 1; i <= 6; ++i) pow10[i] = pow10[i-1] * 10ULL;

    for (int L = 10; L <= 16; ++L) {
        int free = L - 10;                     /* number of free digits */
        int pos_max = L - 10;                  /* last possible start of the substring */
        uint64_t combos = pow10[free];         /* 10^free values */

        /* Fill all positions that are not part of the substring for each start. */
        int others[20];
        for (int pos = 0; pos <= pos_max; ++pos) {
            int oc = 0;
            for (int i = 0; i < L; ++i) {
                if (i < pos || i >= pos + 10)
                    others[oc++] = i;
            }

            for (uint64_t combo = 0; combo < combos; ++combo) {
                uint64_t tmp = combo;
                uint8_t digits[20];

                /* assign digits to the free positions (least significant first) */
                for (int i = 0; i < oc; ++i) {
                    digits[others[i]] = (uint8_t)(tmp % 10);
                    tmp /= 10;
                }

                /* insert the fixed substring */
                for (int i = 0; i < 10; ++i)
                    digits[pos + i] = SUB[i];

                /* leading zero check */
                if (digits[0] == 0) continue;

                /* quick small-prime divisibility tests */
                /* last digit */
                uint8_t last_digit = digits[L-1];
                if ((last_digit % 2 == 0) || (last_digit == 5)) continue;

                /* digit sum mod 3 */
                int sum = 0;
                for (int i = 0; i < L; ++i) sum += digits[i];
                if (sum % 3 == 0) continue;

                /* construct the value */
                uint64_t val = 0;
                for (int i = 0; i < L; ++i) val = val * 10 + digits[i];

                /* primality test for 64‑bit */
                if (is_prime(val))
                    push_prime(val);
            }
        }
    }

    /* Sort and deduplicate the prime list */
    if (prime_size > 1) {
        qsort(prime_list, prime_size, sizeof(seq_t), cmp_seq);
        size_t unique = 1;
        for (size_t i = 1; i < prime_size; ++i) {
            if (prime_list[i] != prime_list[unique-1]) {
                prime_list[unique++] = prime_list[i];
            }
        }
        prime_size = unique;
    }

    precomputed = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1; ignore n=0 */
    precompute_primes();
    if (n <= prime_size) {
        return prime_list[n-1];
    }
    /* The sequence continues beyond the generated range; if requested
       index exceeds the generated primes, return 0 (undefined). */
    return 0;
}
