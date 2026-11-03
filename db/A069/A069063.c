/*
 * A069063  Composite k such that A029939(k) > k*(k+1)/2 where A029939(k) =
 *          Sum_{d|k} phi(d)^2.
 * https://oeis.org/A069063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th composite integer k such that A029939(k) =
 *            Σ_{d|k} φ(d)^2 is greater than k*(k+1)/2
 * Range:     u8 none, u16 n <= 51, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- library includes ----------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- factorisation ----------------------------------------------------- */
static void factorise(uint64_t n, uint64_t *pf, int *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;

    if (m % 2ULL == 0ULL) {
        int e = 0;
        while (m % 2ULL == 0ULL) { m /= 2ULL; ++e; }
        pf[(*cnt)] = 2ULL; exps[(*cnt)] = e; (*cnt)++;
    }

    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            int e = 0;
            while (m % p == 0ULL) { m /= p; ++e; }
            pf[(*cnt)] = p;   exps[(*cnt)] = e; (*cnt)++;
        }
    }

    if (m > 1ULL) {
        pf[(*cnt)] = m;   exps[(*cnt)] = 1; (*cnt)++;
    }
}

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

/* --- helper: composite test ------------------------------------------ */
static bool is_composite(const uint64_t *pf, const int *exps, size_t cnt)
{
    /* k is prime iff it has exactly one prime factor of exponent 1 */
    return !(cnt == 1 && exps[0] == 1);
}

/* --- compute Σ φ(d)² for all divisors of k ---------------------------- */
static seq_t phi_sum_sq_from_factors(const uint64_t *pf, const int *exps, size_t cnt)
{
    seq_t total = 1;  /* multiplicative identity for product */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pf[i];
        int     e = exps[i];

        /* S(p^e) = Σ_{i=0..e} φ(p^i)² */
        seq_t sp = 1;          /* i = 0 term (φ(1) = 1) */
        seq_t power = 1;       /* p^(i-1) for i≥1, start with p^0 */

        for (int k = 1; k <= e; ++k) {
            /* φ(p^k) = p^(k-1) * (p-1) */
            seq_t phi = power * (seq_t)(p - 1);
            sp += phi * phi;
            power *= (seq_t)p;
        }

        total *= sp;
    }

    return total;
}

/* --- mutable state ---------------------------------------------------- */
static seq_t *terms   = NULL;        /* array of computed terms       */
static size_t term_cnt = 0;          /* number of terms currently stored */
static size_t term_cap = 0;          /* allocated capacity            */
static uint64_t current_k = 4;       /* next integer to inspect       */

static void push_term(seq_t val)
{
    grow_seq(&terms, &term_cap, term_cnt + 1);
    terms[term_cnt++] = val;
}

/* --- ensure that at least 'need' terms have been generated ------------ */
static void ensure_terms(size_t need)
{
    while (term_cnt < need) {
        uint64_t pf[20];
        int     exps[20];
        size_t  cnt;

        factorise(current_k, pf, exps, &cnt);

        /* skip primes */
        if (!is_composite(pf, exps, cnt)) {
            current_k++;
            continue;
        }

        seq_t sum  = phi_sum_sq_from_factors(pf, exps, cnt);
        seq_t rhs  = (seq_t)current_k * ((seq_t)current_k + 1) / 2;

        if (sum > rhs) {
            push_term((seq_t)current_k);
        }

        current_k++;
    }
}

/* --- public entry point requested by the build system ---------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */
    ensure_terms((size_t)n);
    return terms[n - 1];
}
