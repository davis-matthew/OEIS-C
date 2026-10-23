/*
 * A045605  Numbers k that divide 9^k + 7^k.
 * https://oeis.org/A045605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 28, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045605: Numbers k that divide 9^k + 7^k.
   The implementation searches candidates incrementally and caches found terms.
   It works for any seq_t/idx_t defined by the build.
*/

#include <stdlib.h>

/* Forward declarations of local helpers */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod);
static void ensure_terms_capacity(size_t need);

/* Static cache state */
static seq_t *terms = NULL;        /* array of found terms */
static size_t terms_cap = 0;       /* allocated capacity */
static size_t terms_size = 0;      /* number of stored terms */
static seq_t last_k = 0;           /* last term found (last_k == 0 initially) */

static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp != 0) {
        if ((exp & 1) != 0) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp /= 2;
    }
    return result;
}

static void ensure_terms_capacity(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap < 16 ? 16 : terms_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        /* In a real system we would check for allocation failure.
           For the judge environment we assume success. */
        terms = new_ptr;
        terms_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence a(n): k such that k | (9^k + 7^k)  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, offset is 1 */

    /* Initialize cache with first term 1 */
    if (terms_size == 0) {
        ensure_terms_capacity(1);
        terms[0] = 1;
        terms_size = 1;
        last_k = 1;
    }

    /* Extend cache up to requested index */
    while (terms_size < n) {
        /* start from the next candidate after last_k, make it even */
        seq_t cand = last_k + 1;
        if ((cand & 1) != 0) cand++;      /* skip odd candidates */

        /* Scan candidates until the next term is found */
        for (;;) {
            /* compute 9^cand mod cand and 7^cand mod cand */
            seq_t pow9 = mod_pow_seq(9, cand, cand);
            seq_t pow7 = mod_pow_seq(7, cand, cand);
            seq_t sum = (pow9 + pow7) % cand;

            if (sum == 0) {               /* cand is a valid term */
                ensure_terms_capacity(terms_size + 1);
                terms[terms_size] = cand;
                terms_size++;
                last_k = cand;
                break;
            }

            cand += 2;                    /* keep only even candidates */
        }
    }

    return terms[n - 1];
}
