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
 * Helpers:   factorise, grow_seq, is_composite, phi_sum_sq_from_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- library includes ----------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorise.h"
#include "../utils/grow_seq.h"
#include "../utils/is_composite.h"
#include "../utils/phi_sum_sq_from_factors.h"

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
