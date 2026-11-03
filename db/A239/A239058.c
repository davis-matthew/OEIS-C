/*
 * A239058  Numbers whose divisors all appear as a substring in their decimal
 *          expansion.
 * https://oeis.org/A239058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 3561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 7150 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
/* A239058: Numbers whose every divisor appears as a decimal substring of the number. */
/* 
   The function a(idx_t n) returns the n‑th term (n ≥ 1).  It builds the
   sequence on demand, caching already found terms, and checks each new
   candidate by factorising it and testing that all of its positive
   divisors occur as substrings in its decimal representation.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static bool divisor_is_substring(uint64_t divisor, const char *s)
{
    char buf[32];
    /* snprintf is safe for 64‑bit values */
    snprintf(buf, sizeof buf, "%llu", (unsigned long long)divisor);
    return strstr(s, buf) != NULL;
}

/* Recursive test of all divisors built from the prime factorisation. */
static bool check_all_divisors(const uint64_t *primes,
                               const uint64_t *exps,
                               size_t cnt,
                               size_t idx,
                               uint64_t current,
                               const char *s)
{
    if (idx == cnt) {
        return divisor_is_substring(current, s);
    }
    uint64_t prime = primes[idx];
    uint64_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        if (!check_all_divisors(primes, exps, cnt, idx + 1,
                                current * power, s))
            return false;
        power *= prime;
    }
    return true;
}

static bool is_candidate_ok(uint64_t n)
{
    char num_str[32];
    snprintf(num_str, sizeof num_str, "%llu", (unsigned long long)n);

    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);
    return check_all_divisors(primes, exps, pcnt, 0, 1, num_str);
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t terms_cnt = 0;   /* number of cached terms */
    static size_t terms_cap = 0;   /* allocated capacity */
    static uint64_t next_candidate = 1; /* next integer to test */

    while (terms_cnt < (size_t)n) {
        if (is_candidate_ok(next_candidate)) {
            grow_seq(&terms, &terms_cap, terms_cnt + 1);
            terms[terms_cnt] = (seq_t)next_candidate;
            ++terms_cnt;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
