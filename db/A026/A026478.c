/*
 * A026478  a(n) = least positive integer > a(n-1) and not of form
 *          a(i)*a(j)*a(k) for 1<=i<=j<=k<n.
 * https://oeis.org/A026478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>=2, a(n) is the least integer > a(n‑1) whose
 *            number of prime factors (with multiplicity) is congruent to 1
 *            modulo 3
 * Range:     u8 n <= 91, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static storage for computed terms and auxiliary tables */
static seq_t  *seq = NULL;       /* array of sequence terms */
static idx_t  seq_len = 0;       /* number of terms currently computed */

static uint32_t *spf = NULL;     /* smallest prime factor table */
static size_t   spf_limit = 0;   /* current maximum index covered by spf */

/* Build a smallest‑prime‑factor table up to |limit| (inclusive). */
static void build_spf(size_t limit)
{
    uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!new_spf) return; /* allocation failure – should not happen for these sizes */

    for (size_t i = 2; i <= limit; ++i) new_spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i)
        if (new_spf[i] == (uint32_t)i) {           /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == (uint32_t)j)
                    new_spf[j] = (uint32_t)i;
        }

    free(spf);
    spf     = new_spf;
    spf_limit = limit;
}

/* Ensure the spf table covers at least |need|; grow if necessary. */
static void ensure_spf(size_t need)
{
    if (spf_limit < need) {
        size_t new_limit = spf_limit * 2 + 10;
        if (new_limit < need * 2 + 10) new_limit = need * 2 + 10;
        build_spf(new_limit);
    }
}

/* Count prime factors with multiplicity (Ω) of x, using the spf table. */
static int omega_count(uint64_t x)
{
    int cnt = 0;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        do {
            x /= p;
            ++cnt;
        } while (x % p == 0);
    }
    return cnt;
}

/* Extend the sequence array up to index n (1‑based). */
static void extend_seq(idx_t n)
{
    if (seq_len == 0) {          /* first call – initialise */
        seq_len = 1;
        seq = (seq_t*)calloc(1, sizeof(seq_t));
        seq[0] = (seq_t)1;       /* a(1) = 1 */
        build_spf(20);           /* small initial spf table */
    }

    if (seq_len >= n) return;    /* already computed */

    /* Grow the term array if needed */
    size_t capacity = seq_len * 2;
    if (capacity < (size_t)n) capacity = (size_t)n;
    seq = (seq_t*)realloc(seq, capacity * sizeof(seq_t));

    seq_t last_term = seq[seq_len - 1];
    seq_t cand = last_term + 1;

    while ((idx_t)seq_len < n) {
        ensure_spf((size_t)cand);
        int omega = omega_count((uint64_t)cand);
        if ((omega % 3) == 1) {
            seq[seq_len++] = cand;
            last_term = cand;
            cand = last_term + 1;
        } else {
            ++cand;
        }
    }
}

/* Main interface required by the template */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for zero, guard */
    extend_seq(n);
    return seq[n - 1];
}
