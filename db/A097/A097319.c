/*
 * A097319  Numbers with more than one prime factor and, in the ordered
 *          factorization, the exponents are strictly increasing.
 * https://oeis.org/A097319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer with at least two distinct
 *            prime * factors and with the exponents in its prime
 *            factorization strictly * increasing when the primes are taken in
 *            ascending order. * * The algorithm generates the sequence
 *            lazily: it keeps a static array * containing all terms found
 *            s...
 * Range:     u8 n <= 11, u16 n <= 292, u32 n <= 1180, u64 n <= 1180,
 *            u128 n <= 1180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const idx_t SIEVE_LIMIT = 2000000;   /* sufficient for n ≤ 10 000 */

/* smallest prime factor array */
static uint32_t *spf = NULL;
static bool sieve_initialized = false;

/* dynamic array of sequence terms */
static seq_t *terms = NULL;
static idx_t terms_len = 0;
static idx_t terms_cap = 0;

/* current integer to test */
static idx_t current_val = 2;

/* fill spf array with linear sieve */
static void init_sieve(void)
{
    if (sieve_initialized) return;

    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);           /* out of memory */
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * 2; j <= SIEVE_LIMIT; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
        }

    sieve_initialized = true;
}

/* add a term to the dynamic array */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        idx_t new_cap = (terms_cap == 0) ? 32 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);      /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* check whether the given integer satisfies the sequence condition */
static bool good_number(idx_t x)
{
    if (x < 4) return false;     /* needs at least two distinct primes */
    idx_t rem = x;
    idx_t prev_exp = 0;
    idx_t prime_cnt = 0;
    bool ok = true;
    while (rem > 1) {
        uint32_t p = spf[rem];
        idx_t exp = 0;
        while (rem % p == 0) {
            ++exp;
            rem /= p;
        }
        ++prime_cnt;
        if (prime_cnt > 1 && exp <= prev_exp) {
            ok = false;
            break;
        }
        prev_exp = exp;
    }
    return ok && prime_cnt > 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safety */

    init_sieve();

    /* generate terms until we reach at least n */
    while (terms_len < n) {
        if (current_val > (idx_t)SIEVE_LIMIT) {
            /* should not happen for the required n range */
            return (seq_t)0;
        }
        if (good_number(current_val))
            add_term((seq_t)current_val);
        ++current_val;
    }
    return terms[n - 1];
}
