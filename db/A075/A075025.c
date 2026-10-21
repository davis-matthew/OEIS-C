/*
 * A075025  Numbers k such that d(k) < d(k-1) and d(k) < d(k+1), where d(k) is
 *          the number of divisors of k.
 * https://oeis.org/A075025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that d(k) < d(k‑1) and d(k) < d(k+1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation builds the sequence lazily.  It extends a */
/*  divisor‑count sieve as needed and stores found local minima in  */
/*  a dynamic array.  All arithmetic uses integer types only.      */

/*  Required headers – the build supplies definitions for seq_t   */
/*  and idx_t. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ----------  dynamic storage for the sequence and its sieve ---------- */

static seq_t *terms = NULL;      /* - produced sequence values            */
static size_t terms_len = 0;     /* - number of terms stored             */
static size_t terms_cap = 0;     /* - capacity of the array               */

static uint32_t *spf = NULL;    /* smallest prime factor array          */
static unsigned char *exp_fact = NULL; /* exponent of that prime for i   */
static uint64_t *tau = NULL;    /* number of divisors of i              */
static size_t spf_cap = 0;      /* capacity of the sieve arrays          */
static size_t cur_limit = 0;    /* highest index processed by the sieve   */

static size_t last_scanned = 1; /* last integer whose local‑min status   */
/*   has been inspected                                                  */

/* ----------  helper functions ------------------------------------- */

/* grow the array that stores the sequence values */
static void grow_terms(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* extend the sieve up to 'limit' (inclusive)                     */
static void extend_to(size_t limit)
{
    if (limit <= cur_limit) return;

    /* allocate more space; +1 to have index 'limit' reachable           */
    size_t new_cap = limit + 1;
    spf     = (uint32_t *)realloc(spf,     new_cap * sizeof(uint32_t));
    exp_fact= (unsigned char *)realloc(exp_fact, new_cap * sizeof(unsigned char));
    tau     = (uint64_t *)realloc(tau,     new_cap * sizeof(uint64_t));
    if (!spf || !exp_fact || !tau) exit(EXIT_FAILURE);

    /* zero the freshly allocated part of the spf array   */
    memset(spf + cur_limit + 1, 0,
           (new_cap - (cur_limit + 1)) * sizeof(uint32_t));

    /* sieve for smallest prime factor (sieve of Eratosthenes)         */
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }

    /* initialise tau[0], tau[1] */
    tau[0] = 0; tau[1] = 1;
    exp_fact[0] = 0; exp_fact[1] = 0;

    /* compute τ(n) using the recurrence on the whole range 2..limit */
    for (size_t i = 2; i <= limit; ++i) {
        uint32_t p  = spf[i];
        size_t   m = i / p;
        if (spf[m] == p) {
            exp_fact[i] = exp_fact[m] + 1;
            tau[i] = tau[m] / (exp_fact[m] + 1ULL) *
                     (exp_fact[i] + 1ULL);
        } else {
            exp_fact[i] = 1;
            tau[i] = tau[m] * 2ULL;
        }
    }

    cur_limit = limit;
}

/* make sure that 'n' terms have been computed                          */
static void ensure_terms(idx_t n)
{
    while (terms_len < (size_t)n) {
        /* enlarge the sieve range, keeping all needed indices       */
        size_t new_limit = cur_limit ? cur_limit * 2 : 1024;
        if (new_limit < last_scanned + 2) new_limit = last_scanned + 2;

        extend_to(new_limit);

        /* scan the newly available integers for local minima     */
        for (size_t k = last_scanned + 1; k <= new_limit - 1; ++k) {
            if (tau[k] < tau[k - 1] && tau[k] < tau[k + 1]) {
                grow_terms(terms_len + 1);
                terms[terms_len++] = (seq_t)k;
            }
        }
        last_scanned = new_limit - 1;
    }
}

/* ----------  exported sequence function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n==0 */
    ensure_terms(n);
    return terms[n - 1];
}
