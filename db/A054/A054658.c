/*
 * A054658  Primes beginning 1, 3, 7, 9 whose reversals are nonprimes.
 * https://oeis.org/A054658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64, reverse_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime whose first digit is 1, 3, 7, or 9 and whose reverse is composite. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/reverse_digits_uint64.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> terms;          /* store already found terms (C++ mode) */
#else
static seq_t *terms = NULL;              /* dynamic array of terms (C mode) */
static size_t terms_cap = 0;
static size_t terms_len = 0;
#endif

/* last integer that was examined while searching for new terms */
static uint64_t last_checked = 1;

/* first digit of a non‑negative integer */
static inline uint64_t first_digit(uint64_t x) {
    while (x >= 10) x /= 10;
    return x;
}

#ifndef __cplusplus
/* enlarge the dynamic array when necessary (C mode only) */
static void ensure_capacity(size_t needed) {
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    /* guard against the undefined offset a(0) */
    if (n == 0) return 0;

    size_t target = (size_t)n;          /* target number of terms */

    /* generate terms until we have at least 'target' of them */
    while (true) {
#ifdef __cplusplus
        if (terms.size() >= target) break;
#else
        if (terms_len >= target) break;
#endif
        last_checked++;

        /* skip non‑primes */
        if (!is_prime_u64(last_checked)) continue;

        /* first digit must be 1, 3, 7, or 9 */
        uint64_t fd = first_digit(last_checked);
        if (fd != 1 && fd != 3 && fd != 7 && fd != 9) continue;

        /* reversal must be composite (non‑prime) */
        uint64_t rev = reverse_digits_uint64(last_checked);
        if (is_prime_u64(rev)) continue;

        /* store the qualified prime */
#ifdef __cplusplus
        terms.push_back(static_cast<seq_t>(last_checked));
#else
        ensure_capacity(terms_len + 1);
        terms[terms_len++] = (seq_t)last_checked;
#endif
    }

#ifdef __cplusplus
    return terms[target - 1];
#else
    return terms[target - 1];
#endif
}
