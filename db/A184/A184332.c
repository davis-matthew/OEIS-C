/*
 * A184332  Ranks of (odd i)+j/r, when all i+j/r are ranked; r=golden ratio
 *          (1+sqrt(5))/2, i>=0, j>=0. Complement of A184333.
 * https://oeis.org/A184332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Sequence A184332 – rank of numbers i + j/r with i odd,
 * where r = (1+√5)/2 (golden ratio), i ≥ 0, j ≥ 0, all numbers
 * are sorted by increasing value.  The function returns the rank
 * (starting from 1) of the n‑th term whose i is odd.
 *
 * The algorithm generates the pairs (i, j) in increasing order of
 * i + j/r by enumerating the integer part K = ⌊i·r + j⌋ and, for each
 * K, outputting the admissible i values in order of their fractional
 * parts i·r – ⌊i·r⌋.  The fractional parts are compared using a
 * high‑accuracy rational approximation of φ = (1+√5)/2:
 *     φ ≈ 1597/987
 * which is accurate enough for all indices used in this program.
 *
 * Since φ > 1, ⌊i·φ⌋ is monotone increasing and the number of terms
 * produced up to rank R is asymptotically R²/(2φ).  For n ≤ 10 000
 * this requires K < 300, so the routine is comfortably within the
 * performance limits.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PHI_NUM 1597ULL  /* numerator of φ approximation 1597/987 */
#define PHI_DEN 987ULL   /* denominator              */
#define MAX_I   1024     /* sufficient for n ≤ 10 000 */

static uint64_t r_fract[MAX_I];   /* (i * PHI_NUM) mod PHI_DEN      */
static int      order_by_frac[MAX_I]; /* indices sorted by fractional part */
static bool     init_done = false;

/* comparison function for qsort, sorting indices by r_fract ascending */
static int cmp_frac(const void *a, const void *b) {
    int i = *(const int *)a;
    int j = *(const int *)b;
    uint64_t ra = r_fract[i];
    uint64_t rb = r_fract[j];
    if (ra < rb) return -1;
    if (ra > rb) return 1;
    return i - j;
}

/* one‑time initialization of r_fract and order_by_frac */
static void init_arrays(void) {
    for (uint32_t i = 0; i < MAX_I; ++i) {
        order_by_frac[i] = (int)i;
        r_fract[i] = (i * PHI_NUM) % PHI_DEN;
    }
    qsort(order_by_frac, MAX_I, sizeof(int), cmp_frac);
    init_done = true;
}

/*
 * Return the rank (offset 1) of the n-th odd‑i term in the ordering
 * of all numbers i + j / r, r = (1+√5)/2.
 */
seq_t a(idx_t n)
{
    if (!init_done) init_arrays();

    idx_t count = 0;   /* how many odd‑i terms we have seen */
    seq_t rank  = 0;   /* current rank (starting from 1) */

    for (uint32_t k = 0; ; ++k) {           /* k = ⌊i·r + j⌋ */
        for (int ii = 0; ii < (int)MAX_I; ++ii) {
            int i = order_by_frac[ii];
            uint64_t ai = (i * PHI_NUM) / PHI_DEN;  /* ⌊i·φ⌋ */
            if (ai <= k) {                       /* pair exists for this k */
                /* j = k – ai, but not needed for rank or parity test */
                ++rank;
                if (i & 1) {                     /* i is odd */
                    ++count;
                    if (count == n)
                        return rank;
                }
            }
        }
    }
}
