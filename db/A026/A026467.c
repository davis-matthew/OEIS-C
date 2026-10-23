/*
 * A026467  a(n) = least positive integer > a(n-1) and not a(i)^2 + a(j)^2 for
 *          1<=i<j<=n.
 * https://oeis.org/A026467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) not equal to a(i)^2 +
 *            a(j)^2 (1 ≤ i < j < n)
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Dynamic bitset for sums of squares */
static uint64_t *sum_bits = NULL;      /* bit array */
static size_t   sum_bits_cap = 0;      /* number of bits stored       */

/* Sequence storage */
static seq_t   *seq  = NULL;           /* array of a(i) values */
static size_t   seq_cap = 0;           /* capacity of seq array */
static size_t   seq_len = 0;           /* current length of sequence */

/* Ensure seq has capacity at least needed */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap > 0 ? seq_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure sum_bits can hold at least idx+1 bits */
static void ensure_sum_bits_cap(seq_t idx)
{
    /* idx must fit in size_t for memory indexing */
    if (idx > (seq_t)SIZE_MAX) return;   /* too large, ignore */
    size_t need = (size_t)idx + 1;
    if (sum_bits_cap >= need) return;
    size_t new_cap = sum_bits_cap > 0 ? sum_bits_cap : 128; /* 128 bits = 16 bytes */
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(sum_bits, new_cap * sizeof(uint64_t));
    if (tmp) {
        /* Zero new bits */
        for (size_t i = sum_bits_cap / 64; i < new_cap / 64; ++i) tmp[i] = 0;
        sum_bits = tmp;
    }
    sum_bits_cap = new_cap;
}

/* Set bit at position idx */
static inline void set_sum_bit(seq_t idx)
{
    if (idx > (seq_t)SIZE_MAX) return;
    ensure_sum_bits_cap(idx);
    size_t pos = (size_t)idx;
    sum_bits[pos >> 6] |= 1ULL << (pos & 63);
}

/* Test if bit at position idx is set */
static inline bool test_sum_bit(seq_t idx)
{
    if (idx > (seq_t)SIZE_MAX) return false;
    size_t pos = (size_t)idx;
    if (pos >= sum_bits_cap) return false;
    return (sum_bits[pos >> 6] & (1ULL << (pos & 63))) != 0;
}

/* Append a new term; compute all new sums involving this term */
static void append_term(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = val;

    /* Pre-calculate square of the new term */
    unsigned long long sq_val = (unsigned long long)val * (unsigned long long)val;

    /* Update sums with previous terms */
    for (size_t i = 0; i + 1 < seq_len; ++i) {
        unsigned long long sq_i = (unsigned long long)seq[i] * (unsigned long long)seq[i];
        seq_t sum = (seq_t)(sq_i + sq_val);
        set_sum_bit(sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* Undefined; return 0 as safe fallback */

    /* Initialize first term if sequence is empty */
    if (seq_len == 0) {
        append_term((seq_t)2);   /* a(1) = 2 */
    }

    /* Extend the sequence up to required n */
    while (seq_len < (size_t)n) {
        seq_t last = seq[seq_len - 1];
        seq_t candidate = last + 1;

        /* Find the next unused candidate */
        while (test_sum_bit(candidate)) {
            ++candidate;
        }

        /* Append the found candidate */
        append_term(candidate);
    }

    return seq[n - 1];
}
