/*
 * A065155  Numbers whose cototient of totient is strictly greater than
 *          totient of cototient.
 * https://oeis.org/A065155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065155:
 * Numbers k such that φ(k)−φ(φ(k)) > φ(k−φ(k)).
 * The function stores discovered terms and returns the nth term
 * (1‑indexed).  Terms are generated on demand and the routine
 * retains state between calls.  The implementation builds an
 * array of smallest prime factors (SPF) up to the current search
 * limit and uses it to compute φ(x) quickly.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----- helper: build smallest prime factor table up to `limit` ----- */
static uint32_t *spf   = NULL;   /* SPF array, 0 <= spf[i] <= i */
static size_t spf_limit = 0;     /* current maximum index populated */

static void build_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) return;          /* out‑of‑memory: caller will ignore result */
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {        /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* ----- helper: compute φ(x) using the SPF table ----- */
static seq_t phi_u64(uint64_t x)
{
    if (x == 0) return 0;        /* φ(0) is defined as 0 for safety */
    seq_t result = (seq_t)x;
    while (x > 1) {
        uint32_t p = spf[x];
        result = result / p * (p - 1);
        while (x % p == 0) x /= p;
    }
    return result;
}

/* ----- storage for discovered sequence terms ----- */
static seq_t *terms = NULL;
static size_t terms_len = 0;   /* number of terms stored */
static size_t terms_cap = 0;   /* allocated capacity */
static uint64_t current_k = 0; /* last integer we tested */

/* grow the terms array if necessary */
static void push_term(uint64_t value)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 128;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed – ignore */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)value;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range – not used by tests */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Ensure SPF table covers the next candidate. */
        if (current_k + 1 > spf_limit)
            build_spf((current_k + 1) * 2 + 100);

        ++current_k;                        /* candidate k */

        seq_t phi_k      = phi_u64(current_k);
        seq_t cot_k      = (seq_t)current_k - phi_k;   /* k - φ(k) */
        seq_t phi_phi_k  = phi_u64((uint64_t)phi_k);
        seq_t phi_cot_k  = phi_u64((uint64_t)cot_k);

        if (phi_k - phi_phi_k > phi_cot_k)
            push_term(current_k);
    }

    return terms[n - 1];
}
