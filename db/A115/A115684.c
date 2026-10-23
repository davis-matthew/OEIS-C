/*
 * A115684  Both n and the reverse of n are the product of 2 palindromes
 *          greater than 1.
 * https://oeis.org/A115684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   int_sqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115684: numbers n such that both n and rev(n) are the product
   of two palindromes greater than 1.  The function scans integers from
   the last examined value and stores qualifying terms until the
   requested index n is reached. */

/* Utility headers */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/int_sqrt.h"  /* int_sqrt */
#include "../utils/rev.h"       /* rev */

/* =================  Palindrome factor helper  ================= */

/*  Store all palindromes up to the largest value needed so far.
    All values are kept as uint64_t for safety; they are cast to seq_t
    only when inserted into the result array.
*/
static uint64_t *pal_list = NULL;  /* array of palindrome numbers */
static size_t   pal_used = 0;      /* number of palindromes stored   */
static size_t   pal_cap  = 0;      /* allocated capacity             */
static uint64_t max_pal  = 0;      /* maximum palindrome value stored */

/* Powers of ten up to 10^10 for palindrome construction */
static const uint64_t pow10[12] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL
};

/* If memory is insufficient, grow the array. */
static void ensure_pal_capacity(size_t needed)
{
    if (pal_cap >= needed) return;
    size_t new_cap = pal_cap ? pal_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(pal_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    pal_list = tmp;
    pal_cap  = new_cap;
}

/* Generate all palindromes <= limit and append them to pal_list. */
static void add_palindromes_up_to(uint64_t limit)
{
    if (max_pal >= limit) return;            /* already covered */
    uint64_t cur_len = 1;                     /* current palindrome length to process */

    /* Determine starting length based on current max_pal */
    while (pow10[cur_len] > max_pal && cur_len > 1) cur_len--;

    /* Generate palindromes of length cur_len and above */
    for (; cur_len <= 10; ++cur_len) {        /* length 1..10 covers all uint64_t palindromes */
        uint64_t half = (cur_len + 1) / 2;
        uint64_t start = pow10[half - 1];
        uint64_t end   = pow10[half] - 1;
        for (uint64_t h = start; h <= end; ++h) {
            uint64_t pal;
            if (cur_len % 2 == 0) {           /* even length */
                uint64_t rev = 0, temp = h;
                while (temp) { rev = rev * 10 + (temp % 10); temp /= 10; }
                pal = h * pow10[half] + rev;
            } else {                          /* odd length */
                uint64_t rev = 0, temp = h / 10;
                while (temp) { rev = rev * 10 + (temp % 10); temp /= 10; }
                pal = h * pow10[half - 1] + rev;
            }
            if (pal > limit) return;          /* we have generated required ones */
            if (pal <= max_pal) continue;     /* already stored */
            ensure_pal_capacity(pal_used + 1);
            pal_list[pal_used++] = pal;
            max_pal = pal;                    /* keep track of maximum */
        }
    }
}

/* Return true if x can be expressed as the product of two palindromes > 1. */
static bool has_pal_factor(uint64_t x)
{
    if (x < 4) return false;                 /* smallest product 2*2 */

    uint64_t sqrtx = int_sqrt(x);             /* integer sqrt */
    if (sqrtx > max_pal) add_palindromes_up_to(sqrtx);

    for (size_t i = 0; i < pal_used; ++i) {
        uint64_t p = pal_list[i];
        if (p > sqrtx) break;
        if (p < 2) continue;                 /* we need >1, though 1 is never stored */

        if (x % p == 0) {
            uint64_t q = x / p;
            if (q > 1 && rev(q) == q) return true;
        }
    }
    return false;
}

/* Return true if both v and its decimal reverse have a palindrome factorization. */
static bool qualifies(uint64_t v)
{
    if (!has_pal_factor(v)) return false;
    uint64_t rv = rev(v);
    return has_pal_factor(rv);
}

/* ==================  Sequence storage  ================== */

static seq_t *seq_terms = NULL;   /* array holding qualifying terms   */
static size_t terms_used = 0;     /* number of terms stored          */
static size_t terms_cap  = 0;     /* allocated capacity              */
static uint64_t last_seen = 1;    /* last integer examined           */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_terms_capacity(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_terms = tmp;
    terms_cap = new_cap;
}

/* -----------------  Sequence function  ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n<1 */

    if (!seq_terms) {                /* first call: allocate storage */
        seq_terms = (seq_t *)malloc(256 * sizeof(seq_t));
        if (!seq_terms) exit(1);
        terms_cap = 256;
        /* Pre‑populate small palindrome list to speed early checks */
        add_palindromes_up_to(1000);
    }

    while (terms_used < (size_t)n) {
        ++last_seen;
        if (qualifies(last_seen)) {
            ensure_terms_capacity(terms_used + 1);
            seq_terms[terms_used++] = (seq_t)last_seen;
        }
    }
    return seq_terms[n - 1];
}
