/*
 * A209076  Number of 7-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the pattern z z+1 z+2.
 * https://oeis.org/A209076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)=39, a(2)=1007, a(3)=8008, a(4)=37108, a(5)=125428,
 *            a(6)=344618, a(7)=817594, a(8)=1738838, a(9)=3397900,
 *            a(10)=6206228, a(11)=10727576, a(12)=17711618, a(13)=28130798,
 *            a(14)=43220926, a(15)=64524812, a(16)=93939228, a(17)=133765612,
 *            a(18)=186763798; for n>18, a(n)=5*a(n‑1)-11*a(n‑2)+1...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* largest n expected by the tests */
#define MAX_N 20000

static const seq_t init_vals[18] = {
    39,           1007,          8008,          37108,
    125428,       344618,        817594,        1738838,
    3397900,      6206228,       10727576,      17711618,
    28130798,     43220926,      64524812,      93939228,
    133765612,    186763798
};

static seq_t cache[MAX_N + 1];
static size_t computed_len = 0;  /* largest index that has a valid value      */
static int base_done = 0;        /* 1 when a(1)..a(18) are fully known        */

/* coefficients for the recurrence, indexed by k=1..16; 0 for k=8 */
static const int coeffs[17] = {
    0,  /* dummy 0 index */
    5, -11, 16, -20, 22, -19, 11, 0, -11, 19, -22, 20, -16, 11, -5, 1
};

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return 0;            /* unused but safe          */
    if (N > MAX_N) {
        /* n is beyond preallocated range: clamp (should not happen) */
        /* to avoid overflow we simply return 0 for safety */
        return 0;
    }

    /* initialise small n values if not yet done */
    if (!base_done) {
        size_t start = computed_len + 1;
        if (start == 0) start = 1;   /* ensure we start from 1   */
        for (size_t i = start; i <= 18 && i <= N; ++i)
            cache[i] = init_vals[i - 1];
        computed_len = (N >= 18 ? 18 : N);
        if (N >= 18)
            base_done = 1;
    }

    /* if N is already computed, return it immediately */
    if (N <= computed_len)
        return cache[N];

    /* compute remaining terms up to N */
    for (size_t i = computed_len + 1; i <= N; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 16; ++k) {
            int coeff = coeffs[k];
            if (coeff == 0) continue;                 /* k = 8      */
            seq_t term = cache[i - k] *
                         (seq_t)(coeff > 0 ? coeff : -coeff);
            if (coeff > 0)
                pos += term;
            else
                neg += term;
        }
        cache[i] = pos - neg;  /* pos ≥ neg by the recurrence   */
    }
    computed_len = N;
    return cache[N];
}
