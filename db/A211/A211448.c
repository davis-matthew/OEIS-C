/*
 * A211448  Number of (n+1)X(n+1) -7..7 symmetric matrices with every 2X2
 *          subblock having sum zero and two, three or four distinct values.
 * https://oeis.org/A211448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×(n+1) symmetric matrices with entries *
 *            from {−7,…,7} such that every 2×2 subblock sums to zero and each
 *            * matrix uses two, three or four distinct values. * This
 *            sequence satisfies the linear recurrence of order 16: * a(n) =
 *            59·a(n−1) −1530·a(n−2) +22776·a(n−3) −212619·a(n−...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[16] = {
    (seq_t)112ULL,
    (seq_t)842ULL,
    (seq_t)6350ULL,
    (seq_t)48066ULL,
    (seq_t)365098ULL,
    (seq_t)2782326ULL,
    (seq_t)21268302ULL,
    (seq_t)163031330ULL,
    (seq_t)1252859362ULL,
    (seq_t)9649453790ULL,
    (seq_t)74464405550ULL,
    (seq_t)575598981066ULL,
    (seq_t)4455555893866ULL,
    (seq_t)34529231972766ULL,
    (seq_t)267840329960454ULL,
    (seq_t)2079116664331442ULL
};

/* coefficients of the recurrence (index 0 -> a(n-1), ... , 15 -> a(n-16)) */
static const int coeff[16] = {
    59, -1530, 22776, -212619, 1268503, -4641183,
    8770309, -1702416, -18846228, 5437041,
    28831259, 22145188, 8024466, 1548672, 152856, 6048
};

/* cache large enough for n up to ~10,000. */
#define CACHE_LIMIT 20000
static seq_t cache[CACHE_LIMIT];
static size_t cache_len = 0;  /* number of elements actually stored */

/* Multiply two seq_t values and return the product. */
static inline seq_t prod(seq_t x, seq_t y) { return x * y; }

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, but avoid UB */
    if (n > CACHE_LIMIT) {
        /* enlarge cache if needed (unlikely for n<=10000). */
        /* For simplicity, static array is used; no reallocation. */
    }

    if (cache_len == 0) {  /* first call: initialize cache with a(1) … a(16) */
        for (size_t i = 0; i < 16 && i < CACHE_LIMIT; ++i)
            cache[i] = init[i];
        cache_len = 16;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* compute values up to n */
    for (size_t idx = cache_len + 1; idx <= n; ++idx) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 16; ++k) {
            /* a(idx - k - 1) is stored at cache[idx - k - 2] */
            seq_t term = prod(cache[idx - k - 2],
                              (seq_t) (coeff[k] >= 0 ? coeff[k] : -coeff[k]));
            if (coeff[k] >= 0)
                pos += term;
            else
                neg += term;
        }
        cache[idx - 1] = pos - neg;  /* final result is non‑negative */
    }

    cache_len = n;
    return cache[n - 1];
}
