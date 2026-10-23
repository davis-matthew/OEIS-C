/*
 * A015988  Eight iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that requires exactly eight
 *            reverse‑and‑add steps to reach a palindrome (index 0-based)
 * Range:     u8 none, u16 none, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

static bool requires_eight_steps(seq_t x)
{
    seq_t cur = x;
    for (int step = 1; step <= 8; ++step) {
        seq_t rev = reverse_decimal(cur);
        cur = cur + rev;
        if (is_palindrome(cur, 10)) {
            return step == 8;
        }
    }
    return false;
}

#ifdef __cplusplus
#include <vector>

static std::vector<seq_t> terms_cpp;            /* cached terms (C++ build) */
static seq_t next_candidate_cpp = 1;            /* first candidate to test */
static void ensure_terms_cpp(idx_t n)
{
    while ((idx_t)terms_cpp.size() <= n) {
        while (!requires_eight_steps(next_candidate_cpp))
            ++next_candidate_cpp;
        terms_cpp.push_back(next_candidate_cpp);
        ++next_candidate_cpp;
    }
}

#endif

/* C build caching */
static seq_t *terms_c = NULL;                  /* raw array of terms */
static size_t terms_cap_c = 0;
static size_t terms_cnt_c = 0;
static seq_t next_candidate_c = 1;

static void grow_terms_c(void)
{
    size_t new_cap = terms_cap_c == 0 ? 64 : terms_cap_c << 1;
    seq_t *p = (seq_t *)realloc(terms_c, new_cap * sizeof(seq_t));
    if (!p) abort();
    terms_c = p;
    terms_cap_c = new_cap;
}

static void ensure_terms_c(idx_t n)
{
    while ((idx_t)terms_cnt_c <= n) {
        while (!requires_eight_steps(next_candidate_c))
            ++next_candidate_c;
        if (terms_cnt_c >= terms_cap_c) grow_terms_c();
        terms_c[terms_cnt_c++] = next_candidate_c;
        ++next_candidate_c;
    }
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    ensure_terms_cpp(n);
    return terms_cpp[(size_t)n];
#else
    ensure_terms_c(n);
    return terms_c[(size_t)n];
#endif
}
