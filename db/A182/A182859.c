/*
 * A182859  Numbers n such that A036459(n) is even.
 * https://oeis.org/A182859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer not already in the sequence
 *            such that the number of divisors of that integer is not equal to
 *            any term already in the sequence
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *spf = NULL;           /* smallest prime factor table */
static size_t   spf_cap = 0;           /* capacity of spf table */

static void ensure_spf(size_t limit) {
    if (limit <= spf_cap) return;
    size_t new_cap = limit + 1;
    if (new_cap < 4) new_cap = 4;
    if (spf) {
        spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    } else {
        spf = (uint32_t *)malloc(new_cap * sizeof(uint32_t));
    }
    memset(spf, 0, new_cap * sizeof(uint32_t));
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) spf[i] = (uint32_t)i;
    }
    spf_cap = limit;
}

static seq_t divisor_count(idx_t x) {
    if (x <= 1) return (seq_t)1;
    size_t n = (size_t)x;
    ensure_spf(n);
    size_t result = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        size_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    return (seq_t)result;
}

/* Arrays for storing the sequence and membership table */
static seq_t *seq = NULL;
static size_t seq_cap = 0, seq_size = 0;
static bool  *present = NULL;
static size_t present_cap = 0;

/* Ensure the presence table has space up to index 'up_to' (inclusive) */
static void ensure_present(size_t up_to) {
    if (up_to < present_cap) return;
    size_t new_cap = present_cap ? present_cap : 4;
    while (new_cap <= up_to) new_cap <<= 1;
    present = (bool *)realloc(present, new_cap * sizeof(bool));
    memset(present + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
    present_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but guard */

    static idx_t next_check = 1;   /* global pointer for candidate search */

    while (seq_size < n) {
        /* Find next admissible integer */
        for (;;) {
            /* Make sure presence table covers both candidate and its divisor count */
            ensure_present((size_t)next_check);
            seq_t d = divisor_count((idx_t)next_check);
            ensure_present((size_t)d);

            if (present[(size_t)next_check]) {
                /* candidate already a sequence term – skip it */
                ++next_check;
                continue;
            }
            if (present[(size_t)d]) {
                /* divisor count already seen as a term – skip it */
                ++next_check;
                continue;
            }

            /* Candidate is accepted as the next sequence term */
            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 64;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_size++] = (seq_t)next_check;
            present[(size_t)next_check] = true;
            ++next_check;
            break;
        }
    }
    return seq[n - 1];
}
