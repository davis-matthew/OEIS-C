/*
 * A185700  The number of periods in a reshuffling operation for compositions
 *          of n.
 * https://oeis.org/A185700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of periods in the reshuffling operation for
 *            compositions of n, as defined in OEIS A185700. The function uses
 *            the recurrence described in the OEIS entry, memoising results
 *            for efficiency
 * Range:     u8 n <= 31, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binom, is_prime, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>                    /* memset */

#include "../utils/binom.h"           /* binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/triangular.h"      /* triangular(idx_t r) -> idx_t */
#include "../utils/is_prime.h"        /* is_prime(idx_t n) -> bool */

/* --------- helper: gcd for idx_t ------------------------------------- */
static idx_t gcd_idx_t(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------- global cache ----------------------------------------------- */
static seq_t  *A_cache = NULL;   /* cached values a(0..A_max) */
static bool   *A_used  = NULL;   /* A_used[i]==true if value computed   */
static idx_t   A_max   = 0;      /* highest index stored (inclusive)     */

/* --------- ensure cache capacity ------------------------------------- */
static bool ensure_capacity(idx_t n) {
    if (n <= A_max) return true;

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp_c = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    if (!tmp_c) return false;
    bool *tmp_u = (bool *)realloc(A_used, new_cap * sizeof(bool));
    if (!tmp_u) { free(tmp_c); return false; }

    /* initialise newly allocated slots */
    memset(tmp_u + A_max + 1, 0,
           (new_cap - (A_max + 1)) * sizeof(bool));

    A_cache = tmp_c;
    A_used  = tmp_u;
    A_max   = n;
    return true;
}

/* --------- forward declaration ---------------------------------------- */
static seq_t a_internal(idx_t n);

/* --------- public entry point ----------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* outside OEIS range */
    if (!ensure_capacity(n)) return 0; /* allocation failure */

    if (A_used[n]) return A_cache[n];

    seq_t val = a_internal(n);
    A_cache[n] = val;
    A_used[n]  = true;
    return val;
}

/* --------- recursive computation -------------------------------------- */
static seq_t a_internal(idx_t n) {
    /* find k such that T(k-1) < n <= T(k)  */
    idx_t k = 1;
    while (triangular(k) < n) ++k;
    idx_t j = n - triangular(k - 1);    /* 1 <= j <= k */

    /* special case: n == T(k)  */
    if (j == k) return (k == 1) ? (seq_t)1 : (seq_t)0;

    /* if k is prime */
    if (is_prime(k)) {
        seq_t b = binom(k, j);
        return b / (seq_t)k;          /* division exact by OEIS definition */
    }

    /* composite k */
    seq_t bin = binom(k, j);
    idx_t g = gcd_idx_t(k, j);
    seq_t sumDiv = 0;

    for (idx_t q = 2; q <= g; ++q) {
        if (g % q != 0) continue;
        idx_t k2 = k / q;
        idx_t j2 = j / q;          /* j divisible by q when q|g */
        idx_t idx = triangular(k2 - 1) + j2;
        seq_t sub = a(idx);
        sumDiv += sub * (seq_t)(k / q);
    }

    seq_t numer = bin - sumDiv;
    return numer / (seq_t)k;
}
