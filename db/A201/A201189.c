/*
 * A201189  Number of nX2 0..2 arrays with every row and column running
 *          average nondecreasing rightwards and downwards but some diagonal
 *          running average having a decrease.
 * https://oeis.org/A201189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201189: Number of n×2 0..2 arrays with row/column averages nondecreasing
   but with some diagonal average decreasing.
   Recurrence (empirical, 17‑term):
   a(n) = 10a(n‑1) – 37a(n‑2) + 48a(n‑3) + 55a(n‑4) – 237a(n‑5)
          +186a(n‑6) +209a(n‑7) –450a(n‑8) +150a(n‑9) +280a(n‑10)
          –297a(n‑11) +20a(n‑12) +126a(n‑13) –69a(n‑14) –5a(n‑15)
          +16a(n‑16) –4a(n‑17)
   Initial values:
   a(1)=0, a(2)=0, a(3)=1, a(4)=6, a(5)=24, a(6)=80, a(7)=239,
   a(8)=665, a(9)=1756, a(10)=4459, a(11)=10978, a(12)=26369,
   a(13)=62066, a(14)=143647, a(15)=327756, a(16)=738794, a(17)=1647917. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* sequence undefined for n < 1 */

    /* Initial terms stored in static cache. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t size  = 0;   /* number of terms currently stored */

    /* Initial coefficients */
    static const int coeffs[17] = {
        10, -37, 48, 55, -237, 186, 209,
        -450, 150, 280, -297, 20, 126,
        -69, -5, 16, -4
    };

    /* Initial sequence values (a(1)..a(17)) */
    static const seq_t init[17] = {
        0ULL, 0ULL, 1ULL, 6ULL, 24ULL, 80ULL, 239ULL,
        665ULL, 1756ULL, 4459ULL, 10978ULL, 26369ULL,
        62066ULL, 143647ULL, 327756ULL, 738794ULL, 1647917ULL
    };

    /* Allocate cache only once, and always keep it at least 17 elements. */
    if (size == 0) {
        size = 17;
        cap   = 17;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return (seq_t)0;      /* out‑of‑memory, unlikely in tests */
        for (size_t i = 0; i < size; ++i)
            cache[i] = init[i];
    }

    /* Return already computed value if available */
    if (n <= size)
        return cache[n - 1];

    /* Grow cache as needed */
    if (cap < (size_t)n) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n) newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp)                /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Compute successive terms using the recurrence */
    for (idx_t k = size + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (int j = 0; j < 17; ++j) {
            idx_t idx = k - j - 2;           /* a(k‑1‑j) is at cache[idx] */
            if (coeffs[j] >= 0)
                sum += ((seq_t)coeffs[j]) * cache[idx];
            else
                sum -= ((seq_t)(-coeffs[j])) * cache[idx];
        }
        cache[size++] = sum;
    }

    return cache[n - 1];
}
