/*
 * A079690  Number of 5's in n!.
 * https://oeis.org/A079690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 975, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of decimal digit 5's in n!  (OEIS A079690) */
/* The function grows the factorial as a big integer in base 10000
   and counts the occurrences of the digit 5.  It memoises results
   so that calls with arbitrary ordering are fast. */

#include <stdint.h>
#include <stddef.h>

/* Storage for big‑integer factorial, using base 10000 (4 decimal digits). */
#define FACT_BASE 10000
/* Allocate enough words for up to about 10 000! (needs < 9000 words). */
#define FACT_CAP 20000
static uint32_t fact_digits[FACT_CAP];
static size_t fact_len = 0;

/* Memoised counts of decimal 5's in n!. */
static seq_t *five_counts = NULL;
static size_t counts_cap  = 0;

/* Current n whose factorial is stored in fact_digits. */
static idx_t cur_n = 0;

/* Ensure the arrays have room for at least need+1 entries */
static void ensure_counts_size(idx_t need) {
    if (need < counts_cap)
        return;
    size_t new_cap = (counts_cap == 0) ? 16 : counts_cap;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(five_counts, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* fatal error if allocation fails */
    five_counts = tmp;
    counts_cap = new_cap;
}

/* Multiply the big integer by a small integer k (k>=1). */
static void multiply_by(uint32_t k) {
    uint64_t carry = 0;
    for (size_t i = 0; i < fact_len; ++i) {
        uint64_t prod = (uint64_t)fact_digits[i] * k + carry;
        fact_digits[i] = (uint32_t)(prod % FACT_BASE);
        carry = prod / FACT_BASE;
    }
    while (carry) {
        fact_digits[fact_len] = (uint32_t)(carry % FACT_BASE);
        carry /= FACT_BASE;
        ++fact_len;
    }
}

/* Count decimal digit '5' in the big integer stored in fact_digits. */
static seq_t count_fives_in_digits(void) {
    uint32_t count = 0;
    for (size_t i = 0; i < fact_len; ++i) {
        uint32_t v = fact_digits[i];
        /* each word contains exactly 4 decimal digits */
        for (int j = 0; j < 4; ++j) {
            if (v % 10 == 5) ++count;
            v /= 10;
        }
    }
    return (seq_t)count;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Initialise on the first call */
    if (cur_n == 0 && fact_len == 0 && five_counts == NULL) {
        fact_len = 1;
        fact_digits[0] = 1;      /* 0! = 1 */
        ensure_counts_size(0);
        five_counts[0] = 0;      /* 0! has no '5's */
        cur_n = 0;
    }

    /* Grow factorial and memoised array up to requested n */
    while (cur_n < n) {
        uint32_t k = (uint32_t)(cur_n + 1);
        multiply_by(k);
        ++cur_n;
        ensure_counts_size(cur_n);
        five_counts[cur_n] = count_fives_in_digits();
    }

    return five_counts[n];
}
