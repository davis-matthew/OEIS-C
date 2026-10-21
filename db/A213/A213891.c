/*
 * A213891  Fixed points of the sequence A262212 defined by the minimum number
 *          of 2's in the relation n*[n,2,2,...,2,n] = [x,...,x] between
 *          simple continued fractions.
 * https://oeis.org/A213891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m>=2 such that P(m+1) ≡ 0 (mod m) and
 *            P((m+1)/q) !≡ 0 (mod m) for every prime divisor q of m+1, where
 *            P(k) is the k‑th Pell number. The relation gives A213891 as the
 *            list of such m. The sequence starts with 3, 11, 19, …
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime, mod_pow, mul_mod64_plain, pell_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/mod_pow.h"          /* mul_mod64 is defined in utils/mod_mul.h */
#include "../utils/is_prime.h"         /* is_prime */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/mul_mod64_plain.h"
#include "../utils/pell_mod.h"

/* Global storage for memoized terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t last_candidate = 2;

/* Expand the terms array to at least 'need' capacity. */
static void terms_ensure_capacity(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 2048;
    if (new_cap < need) new_cap = need;
    seq_t *ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!ptr) abort();
    terms = ptr;
    terms_cap = new_cap;
}

/* Generate the next valid term, starting from 'last_candidate'. */
static void generate_next_term(void)
{
    while (1) {
        idx_t cand = last_candidate++;
        /* Only primes need be considered – the sequence consists of primes. */
        if (!is_prime(cand)) continue;

        seq_t mod = (seq_t)cand;
        idx_t nplus1 = cand + 1;
        seq_t pmod = pell_mod(nplus1, mod);
        if (pmod != 0) continue;            /* requirement P(n+1) ≡ 0 (mod n) */

        /* factor n+1 */
        idx_t temp = nplus1;
        idx_t sqrt_temp = _floor_isqrt_u64(temp);
        bool ok = true;
        idx_t twop = 2;
        while (twop <= sqrt_temp && ok) {
            if (temp % twop == 0) {
                /* distinct prime factor twop */
                uint64_t q = twop;
                idx_t sub = nplus1 / q;
                if (pell_mod(sub, mod) == 0) { ok = false; break; }
                while (temp % twop == 0) temp /= twop;
                sqrt_temp = _floor_isqrt_u64(temp);
            }
            twop = (twop == 2) ? 3 : twop + 2;
        }
        if (ok && temp > 1) { /* remaining prime factor temp */
            uint64_t q = temp;
            idx_t sub = nplus1 / q;
            if (pell_mod(sub, mod) == 0) ok = false;
        }
        if (!ok) continue;

        /* candidate accepted */
        terms_ensure_capacity(terms_len + 1);
        terms[terms_len++] = (seq_t)cand;
        return;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but return 0 safely */
    while (terms_len < n) generate_next_term();
    return terms[n - 1];
}
