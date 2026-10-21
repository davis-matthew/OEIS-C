/*
 * A060674  Numbers k such that phi(x) = k has exactly 11 solutions.
 * https://oeis.org/A060674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer k such that φ(x) = k has exactly 11 solutions. */

/* Dependencies from the build – see the problem statement for the list.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system.  */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* Static state: array of growing terms that have exactly 11 pre‑images.  */
static seq_t *seq_array      = NULL;   /* dynamically built array of terms   */
static size_t seq_count      = 0;       /* number of terms stored             */

/* Build the list of terms up to a given upper bound on the totient value.  */
static void build_terms(idx_t limit, size_t want)
{
    /* Memory for φ[0..limit] and counter[0..limit]  */
    const size_t N = (size_t)limit + 1;
    uint32_t *phi   = (uint32_t *)malloc(N * sizeof(uint32_t));
    uint16_t *cnt   = (uint16_t *)malloc(N * sizeof(uint16_t));

    if (!phi || !cnt) {      /* allocation failure – unlikely, exit silently  */
        free(phi);
        free(cnt);
        return;
    }

    /* Initialise φ[i] = i, counter[i] = 0.  */
    for (size_t i = 0; i < N; ++i) {
        phi[i] = (uint32_t)i;
        cnt[i] = 0;
    }

    /* Classic sieve for Euler's totient.  */
    for (size_t p = 2; p < N; ++p) {
        if (phi[p] == (uint32_t)p) {          /* p is prime */
            for (size_t j = p; j < N; j += p)
                phi[j] -= phi[j] / p;
        }
    }

    /* Count how many x have each totient value.  */
    for (size_t x = 2; x < N; ++x) {
        uint32_t k = phi[x];
        if ((size_t)k < N)
            cnt[k]++;
    }

    /* Collect the totient values that occur exactly 11 times.  */
    size_t new_cap = seq_count ? seq_count : 1024;
    while (seq_array && seq_count + 1 > new_cap)
        new_cap <<= 1;

    if (seq_array && new_cap > seq_count)
        seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    else if (!seq_array)
        seq_array = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!seq_array) {      /* allocation failure – give up silently   */
        free(phi);
        free(cnt);
        return;
    }

    for (uint32_t k = 1; k < N; ++k) {
        if (cnt[k] == 11) {
            if (seq_count >= new_cap) {
                new_cap <<= 1;
                seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
                if (!seq_array) break;   /* allocation failure – stop filling   */
            }
            seq_array[seq_count++] = (seq_t)k;
            if (seq_count >= want)    /* we already have enough terms */
                break;
        }
    }

    free(phi);
    free(cnt);
}

/* Initialise the sequence outside the caller's control.  The first call
   chooses an upper bound that is large enough to produce at least 20000
   terms (enough for n ≤ 10000 with generous safety).  If that limit
   turns out to be insufficient, the function will recompute with a
   larger bound.  */
static void init_sequence(void)
{
    if (seq_count != 0)  /* already initialised */
        return;

    /* Initial bounding limit – approximate upper bound for φ values. */
    idx_t limit = 20000000;            /* 20 million  */
    const size_t NEED = 20000;         /* safety margin for n up to ~10 000 */

    for (;;) {
        build_terms(limit, NEED);
        if (seq_count >= NEED)  /* we have enough terms  */
            break;
        /* Not enough terms: double the limit and start over.  */
        limit <<= 1;
    }
}

/* Public function required by the framework.  */
seq_t a(idx_t n)
{
    init_sequence();

    if (n == 0 || n > seq_count)
        return (seq_t)0;            /* out of range – undefined behaviour */

    return seq_array[n - 1];
}
