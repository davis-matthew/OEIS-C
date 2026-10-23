/*
 * A121895  Number of partitions of n into 4 summands a>=b>=c>=d>0 with
 *          integer a/b, b/c and c/d.
 * https://oeis.org/A121895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d>1} A122935(d-1)
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122935(k) = Sum_{c|k, s=k/c>1} (divisor_count(s-1) - 1) */
/* The implementation precomputes divisor counts and the A122935 values lazily. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External types: idx_t and seq_t are defined by the build system. */

static idx_t _divcnt_size = 0;
static unsigned char *_divcnt = NULL;           /* divisor counts for 0.._divcnt_size */

static seq_t *A122 = NULL;                     /* A122934 values for 0.._A122_size-1 */
static idx_t _A122_size = 0;

/* Ensure divisor counts up to 'limit' are available. */
static void _ensure_divcnt(idx_t limit) {
    if (limit <= _divcnt_size) return;
    if (_divcnt) free(_divcnt);
    _divcnt = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            _divcnt[j] += 1;
        }
    }
    _divcnt_size = limit;
}

/* Ensure A122 values up to 'limit' are available. */
static void _ensure_A122(idx_t limit) {
    if (limit < _A122_size) return;
    /* Allocate/resize array */
    A122 = (seq_t*)realloc(A122, (limit + 1) * sizeof(seq_t));
    for (idx_t k = _A122_size; k <= limit; ++k) {
        seq_t sum = 0;
        /* iterate over divisors of k */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t c1 = d;
                idx_t c2 = k / d;
                /* handle c1 */
                if (c1 > 0) {
                    idx_t s1 = k / c1;
                    if (s1 > 1) {
                        sum += (seq_t)_divcnt[s1 - 1] - 1;
                    }
                }
                /* handle c2 if distinct */
                if (c2 != c1) {
                    idx_t s2 = k / c2;
                    if (s2 > 1) {
                        sum += (seq_t)_divcnt[s2 - 1] - 1;
                    }
                }
            }
        }
        A122[k] = sum;
    }
    _A122_size = limit + 1;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* offset is 1, a(1)=0, a(0) unused */

    /* Ensure precomputation for divisors up to n */
    _ensure_divcnt(n - 1);
    _ensure_A122(n - 1);

    seq_t res = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 > 1) {
                res += A122[d1 - 1];
            }
            if (d2 != d1 && d2 > 1) {
                res += A122[d2 - 1];
            }
        }
    }
    return res;
}
