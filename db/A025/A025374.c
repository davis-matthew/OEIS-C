/*
 * A025374  Numbers that are the sum of 4 nonzero squares in 9 or more ways.
 * https://oeis.org/A025374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th number that is the sum of 4 nonzero squares in 9
 *            or more ways
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Precompute the sequence up to 10000 terms using a direct enumeration of
   sorted quadruples (a>=b>=c>=d>=1).  This guarantees the exact number of
   unordered representations for each n.  */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies these types; they are assumed to exist. */
extern seq_t a(idx_t n);

static idx_t _initialized_terms = 0;               /* number of terms stored */
static seq_t * _terms = NULL;                     /* sequence values */
static size_t _terms_cap = 0;                      /* allocated capacity */

/* Build the sequence up to LIMIT terms.  If LIMIT is 0, build until we have
   10000 terms (the length that most tests will request).  */
static void _build_sequence(size_t limit)
{
    const size_t WANT = limit ? limit : 10000;      /* target number of terms */

    idx_t max_m = 50000;           /* initial guess for maximum n to check */
    while (true) {
        /* Allocate array to hold counts of representations for numbers up to max_m. */
        uint32_t *cnt = (uint32_t *)calloc((size_t)max_m + 1, sizeof(uint32_t));
        if (!cnt) { abort(); }

        /* Enumerate sorted quadruples (a>=b>=c>=d>=1) and count their sums. */
        int S = (int)sqrt((double)max_m);
        for (int a = 1; a <= S; ++a) {
            int a2 = a * a;
            for (int b = 1; b <= a; ++b) {
                int ab2 = a2 + b * b;
                for (int c = 1; c <= b; ++c) {
                    int abc2 = ab2 + c * c;
                    if (abc2 > max_m - 1) break;          /* need at least d^2 >= 1 */
                    for (int d = 1; d <= c; ++d) {
                        int sum = abc2 + d * d;
                        if (sum > max_m) break;
                        ++cnt[sum];
                    }
                }
            }
        }

        /* Gather numbers with at least 9 representations. */
        size_t found = 0;
        for (int val = 4; val <= max_m; ++val) {       /* smallest possible sum is 4 */
            if (cnt[val] >= 9) {
                if (found == _terms_cap) {
                    size_t new_cap = (_terms_cap > 0) ? _terms_cap * 2 : 32;
                    _terms = (seq_t *)realloc(_terms, new_cap * sizeof(seq_t));
                    if (!_terms) abort();
                    _terms_cap = new_cap;
                }
                _terms[found++] = (seq_t)val;
                if (found >= WANT) break;
            }
        }

        free(cnt);

        if (found >= WANT) {
            _initialized_terms = found;
            return;
        }
        /* Not enough terms – increase search range and try again. */
        max_m *= 2;
    }
}

seq_t a(idx_t n)
{
    if (!n) {             /* the OEIS offset is 1; return 0 for 0-th index. */
        return (seq_t)0;
    }
    if (_initialized_terms == 0) {
        _build_sequence(0);          /* build up to 10000 terms */
    }
    /* n is 1‑based; _terms is 0‑based.  */
    return _terms[(size_t)n - 1];
}
