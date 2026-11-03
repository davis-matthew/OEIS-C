/*
 * A075635  a(1) = 1 and then group the composite numbers so that the n-th
 *          group contains n numbers with a prime sum: (1), (4, 9), (6, 8,
 *          15), (10, 12, 14, 25), (16, 18, 20, 21, 22), ...
 * https://oeis.org/A075635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075635
 *
 * a(n) is the nth element of the partition of the positive integers into
 * groups of sizes 1,2,3,… such that:
 *   – each group contains composite numbers (the first group is the single
 *     number 1);
 *   – the sum of the numbers in each group is prime.
 *
 * The sequence is generated incrementally; values are memoised so that
 * calls with arbitrary n are answered in O(1) after the required groups
 * have been built.  This implementation works for the normal integer
 * builds and for the arbitrary‑precision build (when seq_t is a bignum
 * class).  It requires the helper function is_prime_big (../utils/is_prime_big.h)
 * for primality testing of numbers beyond 64 bits. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Memoised terms of the sequence. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Next group to generate: its size equals current_group. */
static idx_t current_group = 1;

/*
 * Records which composite numbers have already appeared.
 * `used[i]` is true iff composite number i has been used.
 * The array grows automatically as larger numbers are inspected.
 */
static bool *used = NULL;
static size_t used_limit = 0;

/* Ensure `used` has capacity for indices up to and including `n`. */
static void
ensure_used_limit(size_t n)
{
    if (n <= used_limit)
        return;
    size_t new_limit = n * 2 + 10;
    bool *new_used = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!new_used)
        abort(); /* OOM */
    for (size_t i = 0; i <= new_limit; ++i)
        new_used[i] = false;
    if (used) {
        size_t copy_limit = used_limit < new_limit ? used_limit : new_limit;
        for (size_t i = 0; i <= copy_limit; ++i)
            new_used[i] = used[i];
        free(used);
    }
    used = new_used;
    used_limit = new_limit;
}

/* Append a value to the global `terms` array, expanding it if necessary. */
static void
append_term(seq_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0 ? 128 : terms_cap * 2);
        terms = mem_resize(seq_t, terms, terms_cap);
        if (!terms)
            abort();
    }
    terms[terms_len++] = v;
}

/* Generate the next group, appending its elements to `terms`. */
static void
generate_next_group(void)
{
    idx_t k = current_group;          /* group size */

    /* Group 1 is the special term 1. */
    if (k == 1) {
        append_term((seq_t)1);
        ++current_group;
        return;
    }

    size_t k_size = (size_t)k;   /* helper for array indices */
    seq_t *first = mem_new(seq_t, (k_size - 1));
    if (!first)
        abort();

    seq_t sum = 0;
    size_t assigned = 0;
    idx_t i = 4;   /* smallest composite number */

    /* Pick the first k–1 smallest unused composites. */
    while (assigned < k_size - 1) {
        if ((size_t)i > used_limit)
            ensure_used_limit((size_t)i);
        if (!is_prime_big((seq_t)i) && !used[(size_t)i]) {
            first[assigned] = (seq_t)i;
            sum += (seq_t)i;
            used[(size_t)i] = true;
            ++assigned;
        }
        ++i;
    }

    /* Find the last composite that makes the group sum prime. */
    idx_t start = (idx_t)first[k_size - 2] + 1;
    seq_t last = 0;
    for (idx_t j = start;; ++j) {
        if ((size_t)j > used_limit)
            ensure_used_limit((size_t)j);
        if (!is_prime_big((seq_t)j) && !used[(size_t)j]) {
            seq_t candidate_sum = sum + (seq_t)j;
            if (is_prime_big(candidate_sum)) {
                last = (seq_t)j;
                used[(size_t)j] = true;
                break;
            }
        }
    }

    /* Append the group to the sequence. */
    for (size_t t = 0; t < k_size - 1; ++t)
        append_term(first[t]);
    append_term(last);

    mem_free(first);
    ++current_group;
}

/* ---------------------------------------------------------- */
/* Required externally visible function                       */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined offset, return 0 */

    while (terms_len < (size_t)n) {
        generate_next_group();
    }
    return terms[(size_t)n - 1];
}
