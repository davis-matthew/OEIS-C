/*
 * A036347  Numbers k for which the parity of k and the parity of sopfr(k)
 *          differ, where sopfr is the sum of prime factors with repetition.
 * https://oeis.org/A036347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k for which the parity of k differs from the parity of
 *            sopfr(k) (sum of prime factors with multiplicity). The function
 *            returns the n-th element of this sequence (1‑indexed)
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build system */
typedef uint64_t idx_t;

/* Forward declaration of the function to compute sequence value */
seq_t a(idx_t n);

static seq_t *terms = NULL;    /* array of sequence terms (1‑indexed) */
static size_t terms_len = 0;   /* number of terms stored */

/* Recompute the sequence up to a given numeric limit.
   The term array is rebuilt entirely. */
static void recompute_upto(idx_t limit)
{
    /* 1. Smallest prime factor sieve (spf) */
    uint32_t *spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) return;  /* allocation failed */

    for (idx_t i = 2; i <= limit / i; ++i)
    {
        if (spf[(size_t)i] == 0)
        {
            spf[(size_t)i] = (uint32_t)i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[(size_t)j] == 0)
                    spf[(size_t)j] = (uint32_t)i;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[(size_t)i] == 0) spf[(size_t)i] = (uint32_t)i;  /* remaining primes */

    /* 2. Parity of number of odd prime factors (mod 2) */
    uint8_t *parity = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!parity) { free(spf); return; }

    parity[0] = parity[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        uint32_t p  = spf[(size_t)i];
        idx_t     m = i / p;
        parity[(size_t)i] = parity[(size_t)m] ^ ((p == 2) ? 0 : 1);
    }

    /* 3. Build the sequence array */
    seq_t *new_terms = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!new_terms) { free(spf); free(parity); return; }

    size_t count = 0;
    for (idx_t i = 1; i <= limit; ++i)
    {
        if ( ((i & 1u) ^ (idx_t)parity[(size_t)i]) == 1u )
        {
            new_terms[count++] = (seq_t)i;
        }
    }

    /* 4. Replace old array */
    seq_t *temp = (seq_t *)realloc(new_terms, count * sizeof(seq_t));
    if (temp) new_terms = temp;
    free(terms);
    terms = new_terms;
    terms_len = count;

    /* 5. Cleanup */
    free(spf);
    free(parity);
}

/* Ensure that we have at least n_needed terms computed. */
static void ensure_terms(idx_t n_needed)
{
    if (terms_len >= (size_t)n_needed) return;

    /* Start with a modest limit, then double until enough terms are found. */
    idx_t limit = (n_needed < 1000) ? 2000 : n_needed * 3;
    while (1)
    {
        recompute_upto(limit);
        if (terms_len >= (size_t)n_needed) break;
        limit <<= 1;  /* double the limit */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0; return 0 for safety */
    ensure_terms(n);
    return terms[n - 1];
}
