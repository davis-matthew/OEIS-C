/*
 * A170108  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170108:
   a(0) = 1
   For 1 ≤ n < 38 :  a(n) = 2 + Σ_{k=0}^{n-1} a(k)
   For n ≥ 38   :  a(n) = (n==38?1:0) + Σ_{k=n-38}^{n-1} a(k)   (for n>38 the 1 disappears)  */
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10000   /* sufficient for the tested range */

static seq_t _a_cache[MAX_N + 1];
static idx_t _computed_max = 0;
static bool _initialized = false;

seq_t a(idx_t n)
{
    /* initialise cache with a(0) */
    if (!_initialized) {
        _a_cache[0] = (seq_t)1;
        _computed_max = 0;
        _initialized = true;
    }

    /* if the requested term is already known, return it */
    if (n <= _computed_max) {
        return _a_cache[n];
    }

    /* if n exceeds the pre‑allocated cache we compute it on the fly
       using a circular buffer of the last 38 terms                */
    if (n > MAX_N) {
        enum { WIN = 38 };
        seq_t win[WIN] = {0};
        win[0] = (seq_t)1;      /* a(0) */
        idx_t pos = 0;           /* position of the next term in the buffer */
        /* compute up to n */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t val;
            if (i <= 37) {
                seq_t sum = 0;
                for (idx_t k = 0; k < i; ++k) {
                    sum += win[k];
                }
                val = ((seq_t)2) + sum;
            } else if (i == 38) {
                seq_t sum = 0;
                for (idx_t k = 0; k < WIN; ++k) {
                    sum += win[k];
                }
                val = ((seq_t)1) + sum;
            } else { /* i > 38 */
                seq_t sum = 0;
                /* the last WIN terms are stored in a circular fashion */
                for (idx_t k = 0; k < WIN; ++k) {
                    sum += win[(pos + k) % WIN];
                }
                val = sum;
            }
            win[pos] = val;
            pos = (pos + 1) % WIN;
        }
        return win[(pos + WIN - 1) % WIN];
    }

    /* compute missing terms up to n and store them in the cache */
    for (idx_t i = _computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i <= 37) {
            for (idx_t k = 0; k < i; ++k) {
                sum += _a_cache[k];
            }
            _a_cache[i] = ((seq_t)2) + sum;
        } else if (i == 38) {
            for (idx_t k = 0; k < 38; ++k) {
                sum += _a_cache[k];
            }
            _a_cache[i] = ((seq_t)1) + sum;
        } else { /* i > 38 */
            for (idx_t k = i - 38; k < i; ++k) {
                sum += _a_cache[k];
            }
            _a_cache[i] = sum;
        }
    }
    _computed_max = n;
    return _a_cache[n];
}
