/*
 * A124966  Numbers which can be expressed as the ordered sum of 3 squares in
 *          2 or more different ways.
 * https://oeis.org/A124966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 7948, u32 n <= 7948, u64 n <= 7948,
 *            u128 n <= 7948, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124966: numbers that can be expressed as a²+b²+c² in
   two or more different unordered ways (a ≤ b ≤ c, a,b,c ≥ 0).
   We precompute all such numbers up to a fixed limit and store
   them in a static array. The function returns the (n‑1)th
   element of that array. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const size_t LIMIT = 400000;        /* enough to cover >10 000 terms   */
static bool                 ready  = false;
static seq_t *              terms  = NULL; /* the collected sequence values   */
static size_t               tcnt   = 0;     /* number of terms stored         */
static size_t               tcap   = 0;     /* capacity of the terms array    */
static uint16_t *           counts = NULL; /* counts of unordered reps of sum */

/* ensure the terms array is large enough */
static void grow_terms(void)
{
    if (tcnt >= tcap) {
        size_t new_cap = tcap ? tcap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failed: abort */
        terms = tmp;
        tcap = new_cap;
    }
}

/* initialize the lookup tables on first call */
static void init(void)
{
    /* count unordered triples a≤b≤c with a²+b²+c² ≤ LIMIT */
    counts = (uint16_t *)calloc(LIMIT + 1, sizeof(uint16_t));
    if (!counts) return;                     /* allocation failed: abort */

    for (size_t a = 0; a * a <= LIMIT; ++a) {
        size_t a2 = a * a;
        for (size_t b = a; a2 + b * b <= LIMIT; ++b) {
            size_t s2 = a2 + b * b;
            for (size_t c = b; s2 + c * c <= LIMIT; ++c) {
                size_t sum = s2 + c * c;
                if (counts[sum] < 0xFFFF) ++counts[sum];
            }
        }
    }

    /* collect sums that have at least two unordered representations */
    for (size_t sum = 9; sum <= LIMIT; ++sum) {  /* 9 is first element of sequence */
        if (counts[sum] >= 2) {
            grow_terms();
            terms[tcnt++] = (seq_t)sum;
        }
    }

    ready = true;
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 */
    if (!ready) init();
    if (!ready || (size_t)n > tcnt) return (seq_t)0;  /* out of bounds → 0 */
    return terms[(size_t)n - 1];           /* sequence is 1‑indexed */
}
