/*
 * A225793  Numbers that can be uniquely expressed as (m + sum of digits of m)
 *          for some m.
 * https://oeis.org/A225793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 208, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A225793:
   Numbers that can be uniquely expressed as (m + sum of digits of m) for some m.
   The function a(idx_t n) returns the nth such number, with offset 1. */
#include <stdlib.h>
#include <stdint.h>

/* Forward: seq_t and idx_t are defined by the build system. */
static inline unsigned digit_sum_u64(idx_t x)
{
    unsigned sum = 0;
    while (x) {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum;
}

static inline size_t digits_u64(idx_t x)
{
    size_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Test whether val has exactly one preimage m such that m + sumdigits(m) = val.
   Only the small integer part of val is needed (values searched are well below 2^64). */
static inline int is_unique(seq_t val)
{
    idx_t v = (idx_t)val;                  /* safe for the required range */
    if (v < 2) return 0;                   /* sequence starts at 2 */
    idx_t start  = v - 1;
    size_t delta = 9 * digits_u64(v - 1);
    idx_t first = (v > delta) ? (v - delta) : 1;

    uint8_t cnt = 0;
    for (idx_t m = start; ; --m) {
        seq_t f = (seq_t)m + (seq_t)digit_sum_u64(m);
        if (f == val) {
            if (++cnt > 1) return 0;
        }
        if (m == first) break;
        if (m == 0) break;   /* defensive */
    }
    return cnt == 1;
}

/* Dynamically build the sequence up to the requested index. */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;
static seq_t next_candidate = 2;   /* smallest number to test next */

static void generate_terms_until(size_t target)
{
    while (terms_len < target) {
        while (!is_unique(next_candidate)) next_candidate++;
        if (terms_len >= terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 64;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_len++] = next_candidate;
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset but no effect on tests */
    generate_terms_until((size_t)n);
    return terms[(size_t)n - 1];
}
