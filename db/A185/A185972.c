/*
 * A185972  Array of divisor product arguments appearing in the numerator of
 *          the unique representation of primorials A002110 in terms of
 *          divisor products.
 * https://oeis.org/A185972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = the (n – 2^{r-1} + 1)-th element of the descending list
 *            of the products of all subsets of the first r primes for which
 *            the number of excluded primes is even; r = min{k : 2^k – 1 ≥ n}
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives the numerator arguments in the unique representation
   of primorials A002110 in terms of divisor products.  It is computed
   by enumerating all subsets of the first r primes, selecting those
   with an even number of excluded primes, computing the product of the
   included primes, sorting the products in decreasing order, and
   picking the appropriate element. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const seq_t primes[50] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL
};

/* Return 1 if the binary representation of x contains an even number of bits. */
static inline int popcount_even(idx_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return ((c & 1) == 0);
}

/* Comparator for descending order of seq_t values.  Works for both
   unsigned integer and big‑integer types. */
static int cmp_desc(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1 */

    /* Find the minimal r such that 2^r – 1 ≥ n.  r ≥ 1. */
    idx_t r = 1;
    while (((idx_t)1 << r) - 1 < n) r++;

    /* Number of terms preceding this row: 2^(r-1) – 1. */
    idx_t prev_rows = ((idx_t)1 << (r - 1)) - 1;
    /* Position within the row (1‑based). */
    idx_t m = n - prev_rows;

    size_t val_count = ((size_t)1 << (r - 1));
    seq_t *vals = alloc_seq_array(val_count);
    size_t pos = 0;

    idx_t limit = ((idx_t)1 << r);
    for (idx_t mask = 0; mask < limit; ++mask) {
        if (!popcount_even(mask)) continue;

        seq_t prod = 1;
        for (int i = 0; i < r; ++i) {
            if (!(mask & ((idx_t)1 << i))) {
                prod *= primes[i];
            }
        }
        vals[pos++] = prod;
    }

    /* vals[0..pos-1] now contains all required products.  Sort descending. */
    qsort(vals, pos, sizeof(seq_t), cmp_desc);

    seq_t result = vals[m - 1];   /* m is 1‑based, array indices are 0‑based. */
    free_seq_array(vals);
    return result;
}
