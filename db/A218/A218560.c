/*
 * A218560  Numbers with d distinct ternary digits (d=1,2,3) such that for
 *          each k=1,...,d, some digit occurs exactly k times.
 * https://oeis.org/A218560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   a(n) = the n‑th smallest non‑negative integer whose ternary
 *            representation satisfies: • the number of distinct ternary
 *            digits d is 1, 2 or 3; • for each k = 1 .. d there is a digit
 *            that occurs exactly k times. The sequence is finite; the whole
 *            list is generated once and reused for all calls
 * Range:     u8 n <= 2, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: comparison function compatible with both C and C++.
   Works when seq_t is an integral type or a class providing operator<. */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Generate all terms of A218560.  The maximum term is 714 < 3⁶, so
   only 3⁶ = 729 numbers need to be examined. */
static void generate_terms(seq_t *out, size_t *count)
{
    static const unsigned long long POW3[7] = {
        1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL
    };

    size_t idx = 0;

    /* d = 1  → length 1 (0, 1, 2) */
    for (unsigned long long x = 0; x < 3; ++x)
        out[idx++] = (seq_t)x;

    /* d = 2  → length 3, 3 distinct digits {0,1,2} but only two present,
       and the counts must be 1 and 2. */
    for (unsigned long long x = POW3[2]; x < POW3[3]; ++x) {
        unsigned char freq[3] = {0, 0, 0};
        unsigned long long t = x;
        for (int i = 0; i < 3; ++i) {
            freq[t % 3]++;   /* count digit */
            t /= 3;
        }
        int distinct = 0;
        int cnt1 = 0, cnt2 = 0;
        for (int d = 0; d < 3; ++d) {
            if (freq[d]) {
                ++distinct;
                if (freq[d] == 1) ++cnt1;
                else if (freq[d] == 2) ++cnt2;
            }
        }
        if (distinct == 2 && cnt1 == 1 && cnt2 == 1)
            out[idx++] = (seq_t)x;
    }

    /* d = 3  → length 6, the three digits must appear with counts
       1, 2 and 3 (in any order). */
    for (unsigned long long x = POW3[5]; x < POW3[6]; ++x) {
        unsigned char freq[3] = {0, 0, 0};
        unsigned long long t = x;
        for (int i = 0; i < 6; ++i) {
            freq[t % 3]++;
            t /= 3;
        }
        int distinct = 0;
        int cnt1 = 0, cnt2 = 0, cnt3 = 0;
        for (int d = 0; d < 3; ++d) {
            if (freq[d]) {
                ++distinct;
                if (freq[d] == 1) ++cnt1;
                else if (freq[d] == 2) ++cnt2;
                else if (freq[d] == 3) ++cnt3;
            }
        }
        if (distinct == 3 && cnt1 == 1 && cnt2 == 1 && cnt3 == 1)
            out[idx++] = (seq_t)x;
    }

    *count = idx;
    qsort(out, idx, sizeof(seq_t), cmp_seq_t);
}

/* Public API required by the build system. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[255];
    static size_t seq_len = 0;

    if (!initialized) {
        generate_terms(seq, &seq_len);
        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return 0;            /* out of range – undefined, return 0 */
    return seq[(size_t)(n - 1)];
}
