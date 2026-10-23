/*
 * A036434  Integers which cannot be written as k+tau(k) for some k.
 * https://oeis.org/A036434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are integers that cannot be expressed as k + τ(k) for any
 *            positive integer k. The function computes the nth such integer
 *            (offset 1). It precomputes all τ(k) up to a fixed limit, marks
 *            every m = k + τ(k) as expressible, then collects the remaining
 *            integers. The limit is chosen large enough fo...
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
extern seq_t a(idx_t n);

/* internal helper to ensure we have enough terms precomputed */
static size_t          precomputed_limit  = 1000000;      /* initial limit */
static uint32_t        *tau               = NULL;        /* divisor counts  */
static unsigned char   *expressible       = NULL;        /* mark expressible m  */
static seq_t           *sequence          = NULL;        /* collected a(n)   */
static idx_t            sequence_len      = 0;           /* number of terms    */
static bool             initialized       = false;       /* initialization flag */

/* build τ(k) for 1..limit; O(limit log limit) */
static void compute_tau(size_t limit)
{
    tau          = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    expressible  = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));

    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i) {
            tau[j] += 1;
        }
    }
}

/* mark all m = k + τ(k) as expressible; m must be ≤ limit */
static void mark_expressible(size_t limit)
{
    for (size_t k = 1; k <= limit; ++k) {
        size_t m = k + tau[k];
        if (m <= limit) {
            expressible[m] = 1;
        }
    }
}

/* collect all integers ≤ limit that are NOT expressible */
static void build_sequence(size_t limit)
{
    /* allocate space for up to limit numbers (worst case) */
    sequence = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    sequence_len = 0;

    for (size_t m = 1; m <= limit; ++m) {
        if (!expressible[m]) {
            sequence[sequence_len++] = (seq_t)m;
        }
    }
}

/* ensure at least n terms are available; double limit if needed */
static void ensure_terms(idx_t n)
{
    if (!initialized || sequence_len < n) {
        while (sequence_len < n) {
            /* first time: use precomputed_limit */
            if (!initialized) {
                compute_tau(precomputed_limit);
                mark_expressible(precomputed_limit);
                build_sequence(precomputed_limit);
                initialized = true;
            } else {
                /* need more terms: extend limit roughly by factor 2 */
                size_t new_limit = precomputed_limit * 2;
                free(tau);
                free(expressible);
                free(sequence);
                precomputed_limit = new_limit;
                compute_tau(precomputed_limit);
                mark_expressible(precomputed_limit);
                build_sequence(precomputed_limit);
            }
        }
    }
}

/* Public function required by specification */
seq_t a(idx_t n)
{
    /* n is 1‑based; validate input (>0) */
    if (n == 0) return 0;

    ensure_terms(n);

    /* return nth term (offset 1) */
    return sequence[n - 1];
}
