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
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime, isqrt, mem, mulmod, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"
#include "../utils/mulmod.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Compute Pell number P(k) modulo mod.  Uses fast exponentiation of
   the matrix [[2,1],[1,0]].  Note that k >= 0. */
static seq_t pell_mod(idx_t k, seq_t mod)
{
    if (k == 0) return 0 % mod;
    if (k == 1) return 1 % mod;

    uint64_t m00 = 2, m01 = 1, m10 = 1, m11 = 0;
    uint64_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;
    uint64_t mod64 = (uint64_t)mod;
    uint64_t e = k - 1;
    while (e > 0) {
        if (e & 1ULL) {
            uint64_t t00 = (mulmod(m00, r00, mod64) +
                            mulmod(m01, r10, mod64)) % mod64;
            uint64_t t01 = (mulmod(m00, r01, mod64) +
                            mulmod(m01, r11, mod64)) % mod64;
            uint64_t t10 = (mulmod(m10, r00, mod64) +
                            mulmod(m11, r10, mod64)) % mod64;
            uint64_t t11 = (mulmod(m10, r01, mod64) +
                            mulmod(m11, r11, mod64)) % mod64;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t t00 = (mulmod(m00, m00, mod64) +
                        mulmod(m01, m10, mod64)) % mod64;
        uint64_t t01 = (mulmod(m00, m01, mod64) +
                        mulmod(m01, m11, mod64)) % mod64;
        uint64_t t10 = (mulmod(m10, m00, mod64) +
                        mulmod(m11, m10, mod64)) % mod64;
        uint64_t t11 = (mulmod(m10, m01, mod64) +
                        mulmod(m11, m11, mod64)) % mod64;
        m00 = t00; m01 = t01; m10 = t10; m11 = t11;
        e >>= 1ULL;
    }
    /* First component of r * [P(1), P(0)]^T = [1,0]^T is r00 */
    return (seq_t)r00;
}

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
    seq_t *ptr = mem_resize(seq_t, terms, new_cap);
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
        idx_t sqrt_temp = isqrt(temp);
        bool ok = true;
        idx_t twop = 2;
        while (twop <= sqrt_temp && ok) {
            if (temp % twop == 0) {
                /* distinct prime factor twop */
                uint64_t q = twop;
                idx_t sub = nplus1 / q;
                if (pell_mod(sub, mod) == 0) { ok = false; break; }
                while (temp % twop == 0) temp /= twop;
                sqrt_temp = isqrt(temp);
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
