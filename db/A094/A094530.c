/*
 * A094530  Numbers with exactly one arithmetic progression of four successive
 *          divisors (not necessarily consecutive).
 * https://oeis.org/A094530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   count_four_ap, exists_in_divs, gen_divs_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094530: Numbers with exactly one arithmetic progression of four divisors (not necessarily consecutive) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_four_ap.h"
#include "../utils/exists_in_divs.h"
#include "../utils/gen_divs_rec.h"

/* Generate all primes up to LIMIT (static, once) */
#define PRIMELIMIT 100000
static uint32_t primes[PRIMELIMIT];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    bool comp[PRIMELIMIT + 1];
    memset(comp, 0, sizeof(comp));
    for (int i = 2; i * i <= PRIMELIMIT; ++i) {
        if (!comp[i]) {
            for (int j = i * i; j <= PRIMELIMIT; j += i)
                comp[j] = true;
        }
    }
    for (int i = 2; i <= PRIMELIMIT; ++i)
        if (!comp[i]) primes[prime_cnt++] = i;
    primes_inited = true;
}

/* Factorize n, output primes[] and exps[], set *cnt.  n fits in uint64_t. */
static void factor_uint64(uint64_t n, uint64_t *out_pr, int *out_exp, int *cnt) {
    init_primes();
    *cnt = 0;
    uint64_t tmp = n;
    for (int i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            out_pr[(*cnt)] = p;
            out_exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {
        out_pr[(*cnt)] = tmp;
        out_exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* Checks whether n has exactly one 4-term AP among its divisors */
static bool has_single_four_ap(uint64_t n) {
    uint64_t pr[64];
    int exps[64];
    int pcnt;
    factor_uint64(n, pr, exps, &pcnt);
    static uint64_t divs[2048];
    int div_cnt = 0;
    gen_divs_rec(0, pcnt, 1, pr, exps, divs, &div_cnt);
    /* sort divisors */
    qsort(divs, div_cnt, sizeof(uint64_t), (int(*)(const void*,const void*))strcmp); /* dummy cast, we'll use custom */
    /* Use our own quicksort to avoid missing cast */
    #define CMP(a,b) ((a)<(b)?-1:((a)>(b)?1:0))
    {
        int left, right;
        for (int i = 1; i < div_cnt; ++i) {
            uint64_t key = divs[i];
            left = 0;
            right = i - 1;
            while (left <= right) {
                int mid = (left + right) >> 1;
                if (divs[mid] <= key) left = mid + 1;
                else right = mid - 1;
            }
            memmove(&divs[left + 1], &divs[left], (i - left) * sizeof(uint64_t));
            divs[left] = key;
        }
    }
    #undef CMP
    return count_four_ap(divs, div_cnt) == 1;
}

/* Memoization storage for sequence terms */
static seq_t *terms = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;
static uint64_t next_candidate = 1;

static seq_t extend_and_insert(void) {
    /* Find next number with exactly one 4-term AP */
    while (true) {
        uint64_t cur = next_candidate++;
        if (has_single_four_ap(cur))
            return (seq_t)cur;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure we have at least n terms computed */
    while (term_len < n) {
        seq_t val = extend_and_insert();
        if (term_len == term_cap) {
            size_t new_cap = term_cap ? term_cap * 2 : 16;
            term_cap = new_cap;
            terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
        }
        terms[term_len++] = val;
    }
    return terms[n - 1];
}
