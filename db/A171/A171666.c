/*
 * A171666  Numbers n such that n, n+1 and n+2 have the same number of
 *          divisors, and that number of divisors is larger than 4.
 * https://oeis.org/A171666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 667, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A171666
 * Numbers n such that n, n+1 and n+2 have the same number of divisors,
 * and that number of divisors is larger than 4.
 *
 * Implementation: linear scan with on‑demand divisor counting.
 * The function works for all supported build types (uint* or
 * arbitrary‑precision class) and for all n ≥ 1.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic storage for found terms (indexing starts at 1) */
static seq_t *terms = NULL;
static size_t terms_len = 0, terms_cap = 0;

/* last candidate that was examined */
static uint64_t last_checked = 0;

/* ensure the terms array can hold at least `need` elements */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failed – silently abort */
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* store a new term (n is the original uint64_t value) */
static void store_term(uint64_t n)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = (seq_t)n;
}

/* divisor count for a uint64_t using trial division */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t cnt = 1;
    uint64_t m = n;
    /* factor 2 */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) cnt *= (e + 1);
    /* odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (m > 1) cnt *= 2;
    return cnt;
}

/* make sure we have at least n terms computed */
static void generate_until(idx_t n)
{
    if (terms_len >= (size_t)n) return;
    if (last_checked == 0) last_checked = 1;

    while (terms_len < (size_t)n) {
        uint64_t i = last_checked;
        for (;;) {
            seq_t d0 = divisor_count_u64(i);
            if (d0 > (seq_t)4 &&
                d0 == divisor_count_u64(i + 1) &&
                d0 == divisor_count_u64(i + 2)) {
                store_term(i);
                last_checked = i + 1;
                break;
            }
            ++i;
        }
    }
}

/* OEIS accessor */
seq_t a(idx_t n)
{
    generate_until(n);
    return terms[n - 1];
}
