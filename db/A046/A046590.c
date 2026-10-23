/*
 * A046590  Numerators of the elements to the right of the central elements of
 *          the 1/4-Pascal triangle (by row), excluding 1's.
 * https://oeis.org/A046590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, frac, less
 * Formula:   Numerators of the elements to the right of the central elements
 *            of the 1/4‑Pascal triangle (by row), excluding 1's
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is defined as follows:
   Row 0:             1
   Row 1:             1   1
   Row 2:             1   1/4   1
   For n ≥ 3, each entry is the sum of the two entries above it
   (standard Pascal rule).  The entries are kept in reduced form.
   For the purpose of the sequence, we enumerate the numerators of all
   entries that lie strictly to the right of the central elements of
   each row (indices > floor(r/2)), skipping the ones.
   The sequence is 1‑indexed: a(1)=5 is the numerator of the first
   non‑1 entry found by this rule. */

#include <stdlib.h>
#include <stdint.h>

/* Helper structure for a rational number with denominator a power of two. */
typedef struct {
    seq_t num;     /* numerator (already reduced: no common factor 2) */
    unsigned exp;  /* denominator = 2^exp, exp ≥ 0 */
} rat_t;

/* Add two rationals a and b and store the result in res. */
static void rat_add(const rat_t *a, const rat_t *b, rat_t *res)
{
    unsigned e = (a->exp > b->exp) ? a->exp : b->exp;
    seq_t na = a->num;
    seq_t nb = b->num;

    /* Shift the numerators to the common denominator 2^e. */
    if (e > a->exp) na <<= (e - a->exp);
    if (e > b->exp) nb <<= (e - b->exp);

    seq_t n = na + nb;

    /* Reduce by removing all common factors of 2. */
    while (e > 0 && !(n & 1)) {
        n >>= 1;
        e--;
    }

    res->num = n;
    res->exp = e;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is 1‑indexed */

    idx_t target = n;                     /* we need the target‑th term */
    idx_t count  = 0;                     /* terms found so far */
    seq_t answer = 0;

    /* Row 2 (the special second row). */
    const idx_t r_start = 2;
    size_t prev_len = r_start + 1;        /* 3 elements */
    rat_t *prev = (rat_t *)malloc(prev_len * sizeof(rat_t));
    if (!prev) return 0;                  /* unlikely: out of memory */

    prev[0].num = 1;  prev[0].exp = 0;
    prev[1].num = 1;  prev[1].exp = 2;    /* 1/4 */
    prev[2].num = 1;  prev[2].exp = 0;

    /* Generate rows starting from r = 3 until we have collected target terms. */
    for (idx_t r = 3; ; ++r) {
        size_t len = r + 1;               /* number of elements in this row */
        rat_t *curr = (rat_t *)malloc(len * sizeof(rat_t));
        if (!curr) { free(prev); return 0; }  /* allocation failure */

        /* Edges are always 1. */
        curr[0].num = 1; curr[0].exp = 0;
        curr[len-1].num = 1; curr[len-1].exp = 0;

        /* Interior entries: sum of two entries above. */
        for (size_t k = 1; k < len-1; ++k) {
            rat_add(&prev[k-1], &prev[k], &curr[k]);
        }

        /* Process entries strictly to the right of the central elements. */
        size_t start = (r / 2) + 1; /* indices > floor(r/2) */
        for (size_t i = start; i < len; ++i) {
            /* Skip entries equal to 1. */
            if (!(curr[i].exp == 0 && curr[i].num == 1)) {
                ++count;
                if (count == target) {
                    answer = curr[i].num;
                    free(curr);
                    free(prev);
                    return answer;
                }
            }
        }

        /* Prepare for the next iteration. */
        free(prev);
        prev = curr;   /* curr becomes the previous row for next round */
    }

    /* Should never reach here. */
    free(prev);
    return answer;
}
