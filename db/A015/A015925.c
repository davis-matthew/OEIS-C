/*
 * A015925  Positive integers n such that 2^n == 2^5 (mod n).
 * https://oeis.org/A015925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 860, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A015925:
   positive integers n such that 2^n ≡ 2^5 (mod n). */

#include <stdint.h>
#include <stdlib.h>

/* compute (a*b) mod m using 128‑bit arithmetic */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* fast modular exponentiation: (base^exp) mod mod, all 64‑bit */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = mul_mod64(result, b, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms  = NULL;   /* array of computed terms */
    static size_t terms_len = 0;   /* number of terms stored   */
    static size_t terms_cap = 0;   /* allocated capacity       */
    static idx_t  next_candidate = 1; /* next integer to test  */

    /* maximal value fitting in seq_t */
    const seq_t seq_t_max = (seq_t)-1;

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        uint64_t cand = (uint64_t)next_candidate;

        /* compute 2^cand mod cand */
        uint64_t val = mod_pow64(2, cand, cand);

        /* RHS: 32 mod cand */
        uint64_t rhs = (cand <= 32) ? 32 % cand : 32;

        if (val == rhs) {
            /* store the candidate if it fits in seq_t */
            if (cand <= (uint64_t)seq_t_max) {
                if (terms_len == terms_cap) {
                    size_t new_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* out of memory */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)cand;
            }
            /* if cand is too large for seq_t we simply ignore it;
               the sequence will not be requested beyond this point. */
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
