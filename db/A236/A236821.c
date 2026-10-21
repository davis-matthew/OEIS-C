/*
 * A236821  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -2*a(n-2) -167*a(n-3) +265*a(n-4) +1110*a(n-5)
 *            -2280*a(n-6) -3350*a(n-7) +7924*a(n-8) +4700*a(n-9)
 *            -13072*a(n-10) -2560*a(n-11) +9856*a(n-12) +128*a(n-13)
 *            -2560*a(n-14) Initial values: a(1)=81, a(2)=327, a(3)=1386,
 *            a(4)=6093, a(5)=27079, a(6)=121590, a(7)=549173, a(8)=2495215,
 *            a(9...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t initial[14] = {
    (seq_t)81ULL,   (seq_t)327ULL,   (seq_t)1386ULL,  (seq_t)6093ULL,
    (seq_t)27079ULL,(seq_t)121590ULL, (seq_t)549173ULL, (seq_t)2495215ULL,
    (seq_t)11389842ULL,(seq_t)52223381ULL,(seq_t)240352711ULL,
    (seq_t)1110041358ULL,(seq_t)5141952693ULL,(seq_t)23882414527ULL
};

static seq_t *cache = NULL;     /* cache[i] holds a(i) */
static size_t allocated = 0;    /* capacity of cache array */
static size_t computed = 0;     /* largest n for which a(n) is computed (>=0) */

/* Ensure that a(k) is computed for all k ≤ n */
static void compute_to(idx_t n)
{
    if (n <= (idx_t)computed)
        return;

    /* Allocate or grow storage */
    size_t new_alloc = (size_t)n + 1;
    if (new_alloc > allocated) {
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – leave state unchanged */
            return;
        }
        cache = tmp;
        allocated = new_alloc;
    }

    /* Fill the first 14 values if not already done */
    if (computed < 14) {
        size_t start = computed + 1;
        size_t end   = (size_t)n < 14 ? (size_t)n : 14;
        for (size_t i = start; i <= end; ++i)
            cache[i] = initial[i - 1];
        computed = end;
    }

    /* Compute from the next index up to n using the recurrence */
    if (n > 14) {
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            seq_t pos = 0, neg = 0;
            /* positive terms */
            pos += cache[i - 1] * (seq_t)9;
            pos += cache[i - 4] * (seq_t)265;
            pos += cache[i - 5] * (seq_t)1110;
            pos += cache[i - 8] * (seq_t)7924;
            pos += cache[i - 9] * (seq_t)4700;
            pos += cache[i -12] * (seq_t)9856;
            pos += cache[i -13] * (seq_t)128;
            /* negative terms */
            neg += cache[i - 2] * (seq_t)2;
            neg += cache[i - 3] * (seq_t)167;
            neg += cache[i - 6] * (seq_t)2280;
            neg += cache[i - 7] * (seq_t)3350;
            neg += cache[i -10] * (seq_t)13072;
            neg += cache[i -11] * (seq_t)2560;
            neg += cache[i -14] * (seq_t)2560;

            cache[i] = pos - neg;   /* pos > neg by construction */
        }
        computed = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    compute_to(n);
    return cache[(size_t)n];
}
