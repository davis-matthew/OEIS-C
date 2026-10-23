/*
 * A023748  Plaindromes: numbers whose digits in base 6 are in nondecreasing
 *          order.
 * https://oeis.org/A023748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 608, u32 n <= 2800, u64 n <= 2800,
 *            u128 n <= 2800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequencing: a(n) is the nth non‑negative integer whose base‑6 digits
 * are in nondecreasing order.  Numbers are sorted first by their length
 * (shorter numbers first), then lexicographically within each length.
 *
 * The implementation uses combinatorial unranking to compute the n‑th
 * element in O(length) time.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t binomial_small(int n, int k)
{
    if (k < 0 || k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset: a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* remaining holds the position after the leading zero (1‑based) */
    seq_t remaining = (seq_t)(n - 1);

    int k;                      /* length of the number in base‑6 */

    /* find length k such that cumulative counts reach n */
    for (k = 1; ; ++k) {
        seq_t cnt = binomial_small(k + 4, 4);   /* C(k+4,4) numbers of length k */
        if (remaining <= cnt) break;
        remaining -= cnt;
    }

    /* zero‑based index within the block of length k */
    seq_t idx = remaining - 1;

    /* Unrank the lexicographic position to obtain the digits. */
    int r = k;            /* remaining positions to fill */
    int min_d = 1;        /* smallest allowed digit (digits are 1…5) */
    seq_t val = (seq_t)0; /* value in base‑10, built while converting base‑6 */

    while (r > 0) {
        for (int d = min_d; d <= 5; ++d) {
            int rem_len = r - 1;                   /* positions left after d */
            int options = 5 - d + 1;               /* digits from d to 5 inclusive */
            seq_t cnt = binomial_small(rem_len + options - 1,
                                        options - 1);   /* number of sequences with this prefix */

            if (idx < cnt) {
                val = val * (seq_t)6 + (seq_t)d;   /* append digit d in base‑6 */
                r = rem_len;
                min_d = d;
                break;
            } else {
                idx -= cnt;          /* skip blocks with smaller first digit */
            }
        }
    }

    return val;
}
